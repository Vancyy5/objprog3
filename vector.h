#ifndef VECTOR_H 
#define VECTOR_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <memory>

template <typename T>
class Vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    pointer data_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;
    size_t realloc_count_ = 0;  // Naujas laukelis perskirstymų skaičiui

    void reallocate(size_type new_capacity);
    void destroy_elements();

public:
    Vector() = default;
    explicit Vector(size_type count, const T& value = T());
    Vector(std::initializer_list<T> init);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    Vector& operator=(std::initializer_list<T> ilist);

    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }
    pointer data() noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }

    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    const_iterator cbegin() const noexcept { return data_; }
    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    const_iterator cend() const noexcept { return data_ + size_; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }
    void reserve(size_type new_cap);
    void shrink_to_fit();

    void clear() noexcept;
    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_type count, const T& value);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_back(const T& value);
    void push_back(T&& value);
    template <class... Args>
    reference emplace_back(Args&&... args);
    void pop_back();
    void resize(size_type count);
    void resize(size_type count, const T& value);
    void swap(Vector& other) noexcept;

    // NEW: assign methods - missing from original implementation
    void assign(size_type count, const T& value);
    template<class InputIt>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> ilist);

    // Nauja funkcija, grąžina perskirstymų skaičių
    size_t reallocations() const { return realloc_count_; }

    friend bool operator==(const Vector& lhs, const Vector& rhs) {
        return lhs.size_ == rhs.size_ && std::equal(lhs.data_, lhs.data_ + lhs.size_, rhs.data_);
    }

    friend bool operator!=(const Vector& lhs, const Vector& rhs) {
        return !(lhs == rhs);
    }
};

// ================= Implementacijos ==================

template <typename T>
void Vector<T>::reallocate(size_type new_capacity) {
    if (new_capacity < size_) new_capacity = size_;
    if (new_capacity == capacity_) return;

    ++realloc_count_;  // Perskirstymo skaičiaus didinimas

    pointer new_data = static_cast<pointer>(::operator new(new_capacity * sizeof(T)));

    for (size_type i = 0; i < size_; ++i) {
        new (&new_data[i]) T(std::move(data_[i]));
        data_[i].~T();
    }

    ::operator delete(static_cast<void*>(data_));
    data_ = new_data;
    capacity_ = new_capacity;
}

template <typename T>
void Vector<T>::destroy_elements() {
    for (size_type i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    size_ = 0;
}

template <typename T>
Vector<T>::Vector(size_type count, const T& value) {
    if (count > 0) {
        data_ = static_cast<pointer>(::operator new(count * sizeof(T)));
        capacity_ = size_ = count;
        for (size_type i = 0; i < count; ++i)
            new (&data_[i]) T(value);
    }
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> init) {
    size_type count = init.size();
    if (count > 0) {
        data_ = static_cast<pointer>(::operator new(count * sizeof(T)));
        capacity_ = size_ = count;
        size_type i = 0;
        for (const auto& el : init) {
            new (&data_[i++]) T(el);
        }
    }
}

template <typename T>
Vector<T>::Vector(const Vector& other) {
    if (other.size_ > 0) {
        data_ = static_cast<pointer>(::operator new(other.size_ * sizeof(T)));
        capacity_ = size_ = other.size_;
        for (size_type i = 0; i < size_; ++i)
            new (&data_[i]) T(other.data_[i]);
    }
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_), realloc_count_(other.realloc_count_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    other.realloc_count_ = 0;
}

template <typename T>
Vector<T>::~Vector() {
    destroy_elements();
    ::operator delete(static_cast<void*>(data_));
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        Vector temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        destroy_elements();
        ::operator delete(static_cast<void*>(data_));

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        realloc_count_ = other.realloc_count_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        other.realloc_count_ = 0;
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> ilist) {
    Vector temp(ilist);
    swap(temp);
    return *this;
}

template <typename T>
typename Vector<T>::reference Vector<T>::at(size_type pos) {
    if (pos >= size_) throw std::out_of_range("Vector::at: index out of range");
    return data_[pos];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::at(size_type pos) const {
    if (pos >= size_) throw std::out_of_range("Vector::at: index out of range");
    return data_[pos];
}

template <typename T>
void Vector<T>::reserve(size_type new_cap) {
    if (new_cap > capacity_) {
        reallocate(new_cap);
    }
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    if (capacity_ > size_) {
        reallocate(size_);
    }
}

template <typename T>
void Vector<T>::clear() noexcept {
    destroy_elements();
}

// NEW: assign method implementations
template <typename T>
void Vector<T>::assign(size_type count, const T& value) {
    clear();
    if (count > capacity_) {
        reallocate(count);
    }
    for (size_type i = 0; i < count; ++i) {
        new (&data_[i]) T(value);
    }
    size_ = count;
}

template <typename T>
template<class InputIt>
void Vector<T>::assign(InputIt first, InputIt last) {
    clear();
    size_type count = std::distance(first, last);
    if (count > capacity_) {
        reallocate(count);
    }
    size_type i = 0;
    for (auto it = first; it != last; ++it, ++i) {
        new (&data_[i]) T(*it);
    }
    size_ = count;
}

template <typename T>
void Vector<T>::assign(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const T& value) {
    return insert(pos, T(value));
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, T&& value) {
    size_type index = pos - data_;
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    for (size_type i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[index] = std::move(value);
    ++size_;
    return data_ + index;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, size_type count, const T& value) {
    if (count == 0) return const_cast<iterator>(pos);

    size_type index = pos - data_;

    if (size_ + count > capacity_) {
        // Naujos talpos dydis (galbūt padvigubinti ar didinti pagal poreikį)
        size_type new_cap = std::max(capacity_ * 2, size_ + count);
        reallocate(new_cap);
    }

    // Perkeliam elementus atgal, nuo galo
    for (size_type i = size_; i > index; --i) {
        new (&data_[i + count - 1]) T(std::move(data_[i - 1]));
        data_[i - 1].~T();
    }

    // Įdedam naujus elementus
    for (size_type i = 0; i < count; ++i) {
        new (&data_[index + i]) T(value);
    }

    size_ += count;

    return data_ + index;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator pos) {
    return erase(pos, pos + 1);
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator first, const_iterator last) {
    size_type start = first - data_;
    size_type end = last - data_;
    size_type count = end - start;

    for (size_type i = start; i < size_ - count; ++i) {
        data_[i] = std::move(data_[i + count]);
    }
    for (size_type i = size_ - count; i < size_; ++i) {
        data_[i].~T();
    }
    size_ -= count;
    return data_ + start;
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (&data_[size_]) T(value);
    ++size_;
}

template <typename T>
void Vector<T>::push_back(T&& value) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (&data_[size_]) T(std::move(value));
    ++size_;
}

template <typename T>
template <class... Args>
typename Vector<T>::reference Vector<T>::emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (&data_[size_]) T(std::forward<Args>(args)...);
    ++size_;
    return back();
}

template <typename T>
void Vector<T>::pop_back() {
    if (size_ > 0) {
        data_[size_ - 1].~T();
        --size_;
    }
}

template <typename T>
void Vector<T>::resize(size_type count) {
    if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            data_[i].~T();
        }
    } else if (count > size_) {
        reserve(count);
        for (size_type i = size_; i < count; ++i) {
            new (&data_[i]) T();
        }
    }
    size_ = count;
}

template <typename T>
void Vector<T>::resize(size_type count, const T& value) {
    if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            data_[i].~T();
        }
    } else if (count > size_) {
        reserve(count);
        for (size_type i = size_; i < count; ++i) {
            new (&data_[i]) T(value);
        }
    }
    size_ = count;
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(realloc_count_, other.realloc_count_);
}

#endif // VECTOR_H