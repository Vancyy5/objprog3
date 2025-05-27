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
    // Member types
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

    void reallocate(size_type new_capacity);
    void destroy_elements();

public:
    // Constructors and destructor
    Vector() = default;
    explicit Vector(size_type count, const T& value = T());
    Vector(std::initializer_list<T> init);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    ~Vector();

    // Assignment operators
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    Vector& operator=(std::initializer_list<T> ilist);

    // Element access
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

    // Iterators
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

    // Capacity
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }
    void reserve(size_type new_cap);
    void shrink_to_fit();

    // Modifiers
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

    // Non-member functions
    friend bool operator==(const Vector& lhs, const Vector& rhs) {
        return lhs.size_ == rhs.size_ && std::equal(lhs.data_, lhs.data_ + lhs.size_, rhs.data_);
    }
    
    friend bool operator!=(const Vector& lhs, const Vector& rhs) {
        return !(lhs == rhs);
    }
};

// Implementation

template <typename T>
void Vector<T>::reallocate(size_type new_capacity) {
    if (new_capacity == 0) {
        destroy_elements();
        delete[] data_;
        data_ = nullptr;
        capacity_ = 0;
        return;
    }

    pointer new_data = new T[new_capacity];
    
    // Move existing elements
    for (size_type i = 0; i < size_; ++i) {
        new_data[i] = std::move(data_[i]);
    }

    destroy_elements();
    delete[] data_;
    
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

// Constructors
template <typename T>
Vector<T>::Vector(size_type count, const T& value) {
    if (count > 0) {
        data_ = new T[count];
        capacity_ = count;
        size_ = count;
        std::fill_n(data_, count, value);
    }
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> init) {
    if (init.size() > 0) {
        data_ = new T[init.size()];
        capacity_ = init.size();
        size_ = init.size();
        std::copy(init.begin(), init.end(), data_);
    }
}

template <typename T>
Vector<T>::Vector(const Vector& other) {
    if (other.size_ > 0) {
        data_ = new T[other.size_];
        capacity_ = other.size_;
        size_ = other.size_;
        std::copy(other.data_, other.data_ + other.size_, data_);
    }
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept 
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template <typename T>
Vector<T>::~Vector() {
    destroy_elements();
    delete[] data_;
}

// Assignment operators
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
        delete[] data_;
        
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> ilist) {
    Vector temp(ilist);
    swap(temp);
    return *this;
}

// Element access
template <typename T>
typename Vector<T>::reference Vector<T>::at(size_type pos) {
    if (pos >= size_) {
        throw std::out_of_range("Vector::at: index out of range");
    }
    return data_[pos];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::at(size_type pos) const {
    if (pos >= size_) {
        throw std::out_of_range("Vector::at: index out of range");
    }
    return data_[pos];
}

// Capacity
template <typename T>
void Vector<T>::reserve(size_type new_cap) {
    if (new_cap > capacity_) {
        reallocate(new_cap);
    }
}

template <typename T>
void Vector<T>::shrink_to_fit() {
    if (size_ < capacity_) {
        reallocate(size_);
    }
}

// Modifiers
template <typename T>
void Vector<T>::clear() noexcept {
    destroy_elements();
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const T& value) {
    size_type index = pos - begin();
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    
    // Shift elements to make space
    for (size_type i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    
    data_[index] = value;
    ++size_;
    return begin() + index;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, T&& value) {
    size_type index = pos - begin();
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    
    // Shift elements to make space
    for (size_type i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    
    data_[index] = std::move(value);
    ++size_;
    return begin() + index;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, size_type count, const T& value) {
    size_type index = pos - begin();
    if (count == 0) return begin() + index;
    
    if (size_ + count > capacity_) {
        reserve(std::max(size_ + count, capacity_ * 2));
    }
    
    // Shift elements to make space
    for (size_type i = size_ - 1; i >= index && i < size_; --i) {
        data_[i + count] = std::move(data_[i]);
    }
    
    // Insert new elements
    std::fill_n(data_ + index, count, value);
    size_ += count;
    return begin() + index;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator pos) {
    size_type index = pos - begin();
    
    // Shift elements to fill the gap
    for (size_type i = index; i < size_ - 1; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }
    
    --size_;
    data_[size_].~T();
    return begin() + index;
}

template <typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator first, const_iterator last) {
    size_type index = first - begin();
    size_type count = last - first;
    
    if (count == 0) return begin() + index;
    
    // Shift elements to fill the gap
    for (size_type i = index; i < size_ - count; ++i) {
        data_[i] = std::move(data_[i + count]);
    }
    
    // Destroy remaining elements
    for (size_type i = size_ - count; i < size_; ++i) {
        data_[i].~T();
    }
    
    size_ -= count;
    return begin() + index;
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = value;
}

template <typename T>
void Vector<T>::push_back(T&& value) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = std::move(value);
}

template <typename T>
template <class... Args>
typename Vector<T>::reference Vector<T>::emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    new (&data_[size_]) T(std::forward<Args>(args)...);
    return data_[size_++];
}

template <typename T>
void Vector<T>::pop_back() {
    if (size_ > 0) {
        data_[--size_].~T();
    }
}

template <typename T>
void Vector<T>::resize(size_type count) {
    if (count > size_) {
        reserve(count);
        for (size_type i = size_; i < count; ++i) {
            new (&data_[i]) T();
        }
    } else if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            data_[i].~T();
        }
    }
    size_ = count;
}

template <typename T>
void Vector<T>::resize(size_type count, const T& value) {
    if (count > size_) {
        reserve(count);
        for (size_type i = size_; i < count; ++i) {
            new (&data_[i]) T(value);
        }
    } else if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            data_[i].~T();
        }
    }
    size_ = count;
}

template <typename T>
void Vector<T>::swap(Vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

// Non-member swap function
template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

#endif // VECTOR_H