#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <memory>
#include <new>

/**
 * @brief Dinaminis šabloninis konteineris, panašus į std::vector.
 * 
 * @tparam T Elementų tipas.
 */
template <typename T>
class Vector {
public:
    using value_type = T;                   ///< Elemento tipas
    using size_type = std::size_t;         ///< Tipas dydžio žymėjimui
    using difference_type = std::ptrdiff_t;///< Skirtumo tipas tarp iteratorių
    using reference = T&;                  ///< Nuoroda į elementą
    using const_reference = const T&;      ///< Konstanta nuoroda į elementą
    using pointer = T*;                    ///< Rodyklė į elementą
    using const_pointer = const T*;        ///< Konstanta rodyklė į elementą
    using iterator = T*;                   ///< Iteratorius
    using const_iterator = const T*;       ///< Konstanta iteratorius
    using reverse_iterator = std::reverse_iterator<iterator>;           ///< Atvirkštinis iteratorius
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; ///< Konstanta atvirkštinis iteratorius

private:
    pointer data_ = nullptr;       ///< Rodyklė į duomenų masyvą
    size_type size_ = 0;           ///< Dabartinis elementų skaičius
    size_type capacity_ = 0;       ///< Dabartinė talpa (alokacijos dydis)
    size_t realloc_count_ = 0;     ///< Kiek kartų buvo perskirstyta atmintis

    /**
     * @brief Atminties perskirstymas naujai talpai.
     * 
     * @param new_capacity Nauja talpa.
     */
    void reallocate(size_type new_capacity) {
        if (new_capacity == 0) {
            destroy_elements();
            std::free(data_);
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            return;
        }

        // Alokuojame naują atmintį
        pointer new_data = static_cast<pointer>(std::malloc(new_capacity * sizeof(T)));
        if (!new_data) {
            throw std::bad_alloc();
        }

        // Perkeliame esamus elementus
        size_type elements_to_move = std::min(size_, new_capacity);
        for (size_type i = 0; i < elements_to_move; ++i) {
            try {
                new (new_data + i) T(std::move(data_[i]));
            } catch (...) {
                // Išvalome jau sukurtus elementus
                for (size_type j = 0; j < i; ++j) {
                    (new_data + j)->~T();
                }
                std::free(new_data);
                throw;
            }
        }

        // Išvalome senuosius elementus ir atmintį
        destroy_elements();
        std::free(data_);

        // Nustatome naują būseną
        data_ = new_data;
        capacity_ = new_capacity;
        size_ = elements_to_move;
        ++realloc_count_;
    }

    /**
     * @brief Naikina visus elementus.
     */
    void destroy_elements() {
        for (size_type i = 0; i < size_; ++i) {
            (data_ + i)->~T();
        }
    }

public:
    /**
     * @brief Numatytasis konstruktorius.
     */
    Vector() = default;

    /**
     * @brief Konstruktorius su elementų skaičiumi ir pradine verte.
     * 
     * @param count Elementų kiekis.
     * @param value Pradinė reikšmė kiekvienam elementui.
     */
    explicit Vector(size_type count, const T& value = T()) {
        if (count > 0) {
            data_ = static_cast<pointer>(std::malloc(count * sizeof(T)));
            if (!data_) {
                throw std::bad_alloc();
            }
            
            capacity_ = count;
            size_ = 0;
            
            for (size_type i = 0; i < count; ++i) {
                try {
                    new (data_ + i) T(value);
                    ++size_;
                } catch (...) {
                    destroy_elements();
                    std::free(data_);
                    data_ = nullptr;
                    size_ = 0;
                    capacity_ = 0;
                    throw;
                }
            }
        }
    }

    /**
     * @brief Konstruktorius iš initializer_list.
     * 
     * @param init Inicilizacijos sąrašas.
     */
    Vector(std::initializer_list<T> init) {
        if (init.size() > 0) {
            data_ = static_cast<pointer>(std::malloc(init.size() * sizeof(T)));
            if (!data_) {
                throw std::bad_alloc();
            }
            
            capacity_ = init.size();
            size_ = 0;
            
            for (const auto& item : init) {
                try {
                    new (data_ + size_) T(item);
                    ++size_;
                } catch (...) {
                    destroy_elements();
                    std::free(data_);
                    data_ = nullptr;
                    size_ = 0;
                    capacity_ = 0;
                    throw;
                }
            }
        }
    }

    /**
     * @brief Kopijavimo konstruktorius.
     * 
     * @param other Kitas vektorius.
     */
    Vector(const Vector& other) {
        if (other.size_ > 0) {
            data_ = static_cast<pointer>(std::malloc(other.size_ * sizeof(T)));
            if (!data_) {
                throw std::bad_alloc();
            }
            
            capacity_ = other.size_;
            size_ = 0;
            
            for (size_type i = 0; i < other.size_; ++i) {
                try {
                    new (data_ + i) T(other.data_[i]);
                    ++size_;
                } catch (...) {
                    destroy_elements();
                    std::free(data_);
                    data_ = nullptr;
                    size_ = 0;
                    capacity_ = 0;
                    throw;
                }
            }
        }
    }

    /**
     * @brief Perkėlimo konstruktorius.
     * 
     * @param other Perkeliamas vektorius.
     */
    Vector(Vector&& other) noexcept 
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_), realloc_count_(other.realloc_count_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        other.realloc_count_ = 0;
    }

    /**
     * @brief Destruktorius.
     */
    ~Vector() {
        destroy_elements();
        std::free(data_);
    }

    // Operatoriai
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            destroy_elements();
            std::free(data_);
            
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

    Vector& operator=(std::initializer_list<T> ilist) {
        Vector temp(ilist);
        swap(temp);
        return *this;
    }

    // Prieiga prie elementų
    /**
     * @brief Prieiga su ribų tikrinimu.
     * 
     * @param pos Indeksas.
     * @return Nuoroda į elementą.
     */
    reference at(size_type pos) {
        if (pos >= size_) {
            throw std::out_of_range("Vector::at: index out of range");
        }
        return data_[pos];
    }

    /**
     * @brief Prieiga su ribų tikrinimu (konstanta).
     * 
     * @param pos Indeksas.
     * @return Konstanta nuoroda į elementą.
     */
    const_reference at(size_type pos) const {
        if (pos >= size_) {
            throw std::out_of_range("Vector::at: index out of range");
        }
        return data_[pos];
    }

    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }
    pointer data() noexcept { return data_; }
    const_pointer data() const noexcept { return data_; }

    // Iteratoriai
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

    // Naudingos funkcijos
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type capacity() const noexcept { return capacity_; }

    void reserve(size_type new_cap) {
        if (new_cap > capacity_) {
            reallocate(new_cap);
        }
    }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            reallocate(size_);
        }
    }

    void clear() noexcept {
        destroy_elements();
        size_ = 0;
    }

    // Modifikavimas
    iterator insert(const_iterator pos, const T& value) {
        size_type index = pos - data_;
        
        if (size_ == capacity_) {
            size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_cap);
        }
        
        // Pastumti elementus į dešinę
        for (size_type i = size_; i > index; --i) {
            new (data_ + i) T(std::move(data_[i - 1]));
            (data_ + i - 1)->~T();
        }
        
        new (data_ + index) T(value);
        ++size_;
        
        return data_ + index;
    }

    iterator insert(const_iterator pos, T&& value) {
        size_type index = pos - data_;
        
        if (size_ == capacity_) {
            size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_cap);
        }
        
        // Pastumti elementus į dešinę
        for (size_type i = size_; i > index; --i) {
            new (data_ + i) T(std::move(data_[i - 1]));
            (data_ + i - 1)->~T();
        }
        
        new (data_ + index) T(std::move(value));
        ++size_;
        
        return data_ + index;
    }

    iterator insert(const_iterator pos, size_type count, const T& value) {
        if (count == 0) return const_cast<iterator>(pos);
        
        size_type index = pos - data_;
        
        if (size_ + count > capacity_) {
            size_type new_cap = std::max(capacity_ * 2, size_ + count);
            reallocate(new_cap);
        }
        
        // Pastumti esamus elementus į dešinę
        for (size_type i = size_ + count - 1; i >= index + count && i < size_ + count; --i) {
            new (data_ + i) T(std::move(data_[i - count]));
            (data_ + i - count)->~T();
        }
        
        // Įdėti naujus elementus
        for (size_type i = 0; i < count; ++i) {
            new (data_ + index + i) T(value);
        }
        
        size_ += count;
        return data_ + index;
    }

    iterator erase(const_iterator pos) {
        size_type index = pos - data_;
        
        (data_ + index)->~T();
        
        // Pastumti elementus į kairę
        for (size_type i = index; i < size_ - 1; ++i) {
            new (data_ + i) T(std::move(data_[i + 1]));
            (data_ + i + 1)->~T();
        }
        
        --size_;
        return data_ + index;
    }

    iterator erase(const_iterator first, const_iterator last) {
        size_type start_index = first - data_;
        size_type end_index = last - data_;
        size_type count = end_index - start_index;
        
        if (count == 0) return const_cast<iterator>(first);
        
        // Sunaikinti elementus intervale
        for (size_type i = start_index; i < end_index; ++i) {
            (data_ + i)->~T();
        }
        
        // Pastumti likusius elementus į kairę
        for (size_type i = start_index; i < size_ - count; ++i) {
            new (data_ + i) T(std::move(data_[i + count]));
            (data_ + i + count)->~T();
        }
        
        size_ -= count;
        return data_ + start_index;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_cap);
        }
        
        new (data_ + size_) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) {
            size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_cap);
        }
        
        new (data_ + size_) T(std::move(value));
        ++size_;
    }

    /**
     * @brief Sukuria objektą gale, panaudojant nurodytus argumentus.
     * 
     * @tparam Args Argumentų tipai.
     * @param args Argumentai, naudojami sukurti objektui.
     * @return Nuoroda į naują objektą.
     */
    template <class... Args>
    reference emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_cap);
        }
        
        new (data_ + size_) T(std::forward<Args>(args)...);
        ++size_;
        return data_[size_ - 1];
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            (data_ + size_)->~T();
        }
    }

    void resize(size_type count) {
        resize(count, T());
    }

    void resize(size_type count, const T& value) {
        if (count < size_) {
            // Sumažinti dydį
            for (size_type i = count; i < size_; ++i) {
                (data_ + i)->~T();
            }
            size_ = count;
        } else if (count > size_) {
            // Padidinti dydį
            if (count > capacity_) {
                reallocate(count);
            }
            
            for (size_type i = size_; i < count; ++i) {
                new (data_ + i) T(value);
            }
            size_ = count;
        }
    }

    void swap(Vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(realloc_count_, other.realloc_count_);
    }

    // Naujos funkcijos
    void assign(size_type count, const T& value) {
        clear();
        if (count > capacity_) {
            reallocate(count);
        }
        
        for (size_type i = 0; i < count; ++i) {
            new (data_ + i) T(value);
        }
        size_ = count;
    }

    template<class InputIt>
    void assign(InputIt first, InputIt last) {
        clear();
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }

    void assign(std::initializer_list<T> ilist) {
        clear();
        if (ilist.size() > capacity_) {
            reallocate(ilist.size());
        }
        
        size_type i = 0;
        for (const auto& item : ilist) {
            new (data_ + i) T(item);
            ++i;
        }
        size_ = ilist.size();
    }

    /**
     * @brief Gauk kiek kartų buvo atlikta atminties perskirstymų.
     * 
     * @return Perskirstymų skaičius.
     */
    size_t reallocations() const {
        return realloc_count_;
    }

    // Lyginimo operatoriai
    friend bool operator==(const Vector& lhs, const Vector& rhs) {
        return lhs.size_ == rhs.size_ && std::equal(lhs.data_, lhs.data_ + lhs.size_, rhs.data_);
    }

    friend bool operator!=(const Vector& lhs, const Vector& rhs) {
        return !(lhs == rhs);
    }
};

#endif // VECTOR_H