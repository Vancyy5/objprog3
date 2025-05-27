#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "vector.h"

// Private helper methods
template<typename T>
void Vector<T>::reallocate(size_type new_capacity) {
    if (new_capacity == 0) {
        destroy_elements();
        std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
        data_ = nullptr;
        capacity_ = 0;
        return;
    }
    
    T* new_data = std::allocator_traits<allocator_type>::allocate(alloc_, new_capacity);
    
    size_type elements_to_move = std::min(size_, new_capacity);
    
    // Move/copy construct elements to new location
    for (size_type i = 0; i < elements_to_move; ++i) {
        if constexpr (std::is_nothrow_move_constructible_v<T>) {
            std::allocator_traits<allocator_type>::construct(alloc_, new_data + i, std::move(data_[i]));
        } else {
            std::allocator_traits<allocator_type>::construct(alloc_, new_data + i, data_[i]);
        }
    }
    
    // Clean up old data
    destroy_elements();
    if (data_) {
        std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
    }
    
    data_ = new_data;
    capacity_ = new_capacity;
    size_ = elements_to_move;
}

template<typename T>
void Vector<T>::destroy_elements() {
    for (size_type i = 0; i < size_; ++i) {
        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
    }
}

// Constructors and destructor
template<typename T>
Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0), alloc_() {}

template<typename T>
Vector<T>::Vector(const allocator_type& alloc) 
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {}

template<typename T>
Vector<T>::Vector(size_type count, const T& value, const allocator_type& alloc) 
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
    if (count > 0) {
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, count);
        capacity_ = count;
        
        for (size_type i = 0; i < count; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, value);
            ++size_;
        }
    }
}

template<typename T>
template<class InputIt>
Vector<T>::Vector(InputIt first, InputIt last, const allocator_type& alloc) 
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
    if constexpr (std::is_same_v<typename std::iterator_traits<InputIt>::iterator_category, 
                                std::random_access_iterator_tag>) {
        // Random access iterator - we can calculate distance
        size_type count = std::distance(first, last);
        if (count > 0) {
            data_ = std::allocator_traits<allocator_type>::allocate(alloc_, count);
            capacity_ = count;
            
            for (auto it = first; it != last; ++it) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, *it);
                ++size_;
            }
        }
    } else {
        // Input iterator - grow as needed
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }
}

template<typename T>
Vector<T>::Vector(const Vector& other) 
    : data_(nullptr), size_(0), capacity_(0), alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.alloc_)) {
    if (other.size_ > 0) {
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, other.size_);
        capacity_ = other.size_;
        
        for (size_type i = 0; i < other.size_; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, other.data_[i]);
            ++size_;
        }
    }
}

template<typename T>
Vector<T>::Vector(const Vector& other, const allocator_type& alloc) 
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
    if (other.size_ > 0) {
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, other.size_);
        capacity_ = other.size_;
        
        for (size_type i = 0; i < other.size_; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, other.data_[i]);
            ++size_;
        }
    }
}

template<typename T>
Vector<T>::Vector(Vector&& other) noexcept 
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_), alloc_(std::move(other.alloc_)) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T>
Vector<T>::Vector(Vector&& other, const allocator_type& alloc) 
    : alloc_(alloc) {
    if (alloc_ == other.alloc_) {
        // Allocators are equal, we can steal the data
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    } else {
        // Allocators are different, we need to move elements individually
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        
        if (other.size_ > 0) {
            data_ = std::allocator_traits<allocator_type>::allocate(alloc_, other.size_);
            capacity_ = other.size_;
            
            for (size_type i = 0; i < other.size_; ++i) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move(other.data_[i]));
                ++size_;
            }
        }
    }
}

template<typename T>
Vector<T>::Vector(std::initializer_list<T> init, const allocator_type& alloc) 
    : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
    if (init.size() > 0) {
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, init.size());
        capacity_ = init.size();
        
        for (const auto& item : init) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, item);
            ++size_;
        }
    }
}

template<typename T>
Vector<T>::~Vector() {
    destroy_elements();
    if (data_) {
        std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
    }
}

// Assignment operators
template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        // Handle allocator propagation
        if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
            if (alloc_ != other.alloc_) {
                // Different allocators, need to deallocate with old allocator
                destroy_elements();
                if (data_) {
                    std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
                }
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
                alloc_ = other.alloc_;
            }
        }
        
        if (other.size_ > capacity_) {
            reallocate(other.size_);
        } else {
            // Destroy excess elements
            for (size_type i = other.size_; i < size_; ++i) {
                std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
            }
        }
        
        // Copy elements
        size_type min_size = std::min(size_, other.size_);
        for (size_type i = 0; i < min_size; ++i) {
            data_[i] = other.data_[i];
        }
        
        // Construct new elements if needed
        for (size_type i = min_size; i < other.size_; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, other.data_[i]);
        }
        
        size_ = other.size_;
    }
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
            // Clean up current data
            destroy_elements();
            if (data_) {
                std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
            }
            
            // Move everything
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            alloc_ = std::move(other.alloc_);
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        } else {
            if (alloc_ == other.alloc_) {
                // Same allocators, can move data directly
                destroy_elements();
                if (data_) {
                    std::allocator_traits<allocator_type>::deallocate(alloc_, data_, capacity_);
                }
                
                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;
                
                other.data_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            } else {
                // Different allocators, need to move elements individually
                if (other.size_ > capacity_) {
                    reallocate(other.size_);
                }
                
                size_type min_size = std::min(size_, other.size_);
                for (size_type i = 0; i < min_size; ++i) {
                    data_[i] = std::move(other.data_[i]);
                }
                
                for (size_type i = min_size; i < other.size_; ++i) {
                    std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move(other.data_[i]));
                }
                
                for (size_type i = other.size_; i < size_; ++i) {
                    std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
                }
                
                size_ = other.size_;
            }
        }
    }
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(std::initializer_list<T> ilist) {
    assign(ilist);
    return *this;
}

// Assign methods
template<typename T>
void Vector<T>::assign(size_type count, const T& value) {
    if (count > capacity_) {
        reallocate(count);
    } else {
        for (size_type i = count; i < size_; ++i) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
        }
    }
    
    size_type min_size = std::min(size_, count);
    for (size_type i = 0; i < min_size; ++i) {
        data_[i] = value;
    }
    
    for (size_type i = min_size; i < count; ++i) {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, value);
    }
    
    size_ = count;
}

template<typename T>
template<class InputIt>
void Vector<T>::assign(InputIt first, InputIt last) {
    clear();
    
    if constexpr (std::is_same_v<typename std::iterator_traits<InputIt>::iterator_category, 
                                std::random_access_iterator_tag>) {
        size_type count = std::distance(first, last);
        if (count > capacity_) {
            reallocate(count);
        }
        
        for (auto it = first; it != last; ++it) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, *it);
            ++size_;
        }
    } else {
        for (auto it = first; it != last; ++it) {
            push_back(*it);
        }
    }
}

template<typename T>
void Vector<T>::assign(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
}

// Element access
template<typename T>
typename Vector<T>::reference Vector<T>::at(size_type pos) {
    if (pos >= size_) {
        throw std::out_of_range("Vector::at: index out of range");
    }
    return data_[pos];
}

template<typename T>
typename Vector<T>::const_reference Vector<T>::at(size_type pos) const {
    if (pos >= size_) {
        throw std::out_of_range("Vector::at: index out of range");
    }
    return data_[pos];
}

// Capacity
template<typename T>
void Vector<T>::reserve(size_type new_cap) {
    if (new_cap > capacity_) {
        reallocate(new_cap);
    }
}

template<typename T>
void Vector<T>::shrink_to_fit() {
    if (size_ < capacity_) {
        reallocate(size_);
    }
}

// Modifiers
template<typename T>
void Vector<T>::clear() noexcept {
    destroy_elements();
    size_ = 0;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const T& value) {
    size_type index = pos - cbegin();
    
    if (size_ == capacity_) {
        size_type new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    // Move elements to make space
    for (size_type i = size_; i > index; --i) {
        if (i == size_) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move(data_[i-1]));
        } else {
            data_[i] = std::move(data_[i-1]);
        }
    }
    
    // Insert new element
    if (index < size_) {
        data_[index] = value;
    } else {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + index, value);
    }
    
    ++size_;
    return iterator(data_ + index);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, T&& value) {
    size_type index = pos - cbegin();
    
    if (size_ == capacity_) {
        size_type new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    // Move elements to make space
    for (size_type i = size_; i > index; --i) {
        if (i == size_) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move(data_[i-1]));
        } else {
            data_[i] = std::move(data_[i-1]);
        }
    }
    
    // Insert new element
    if (index < size_) {
        data_[index] = std::move(value);
    } else {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + index, std::move(value));
    }
    
    ++size_;
    return iterator(data_ + index);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, size_type count, const T& value) {
    if (count == 0) return iterator(const_cast<T*>(&(*pos)));
    
    size_type index = pos - cbegin();
    
    if (size_ + count > capacity_) {
        size_type new_capacity = std::max(size_ + count, capacity_ * 2);
        reallocate(new_capacity);
    }
    
    // Move elements to make space
    for (size_type i = size_ + count - 1; i >= index + count; --i) {
        if (i >= size_) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move(data_[i - count]));
        } else {
            data_[i] = std::move(data_[i - count]);
        }
    }
    
    // Insert new elements
    for (size_type i = 0; i < count; ++i) {
        if (index + i < size_) {
            data_[index + i] = value;
        } else {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + index + i, value);
        }
    }
    
    size_ += count;
    return iterator(data_ + index);
}

template<typename T>
template<class InputIt>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, InputIt first, InputIt last) {
    if (first == last) return iterator(const_cast<T*>(&(*pos)));
    
    size_type index = pos - cbegin();
    
    if constexpr (std::is_same_v<typename std::iterator_traits<InputIt>::iterator_category, 
                                std::random_access_iterator_tag>) {
        size_type count = std::distance(first, last);
        
        if (size_ + count > capacity_) {
            size_type new_capacity = std::max(size_ + count, capacity_ * 2);
            reallocate(new_capacity);
        }
        
        // Move elements to make space
        for (size_type i = size_ + count - 1; i >= index + count; --i) {
            if (i >= size_) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move(data_[i - count]));
            } else {
                data_[i] = std::move(data_[i - count]);
            }
        }
        
        // Insert new elements
        size_type i = 0;
        for (auto it = first; it != last; ++it, ++i) {
            if (index + i < size_) {
                data_[index + i] = *it;
            } else {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + index + i, *it);
            }
        }
        
        size_ += count;
    } else {
        // For input iterators, insert one by one
        auto current_pos = iterator(data_ + index);
        for (auto it = first; it != last; ++it) {
            current_pos = insert(current_pos, *it);
            ++current_pos;
        }
    }
    
    return iterator(data_ + index);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
}

template<typename T>
template<class... Args>
typename Vector<T>::iterator Vector<T>::emplace(const_iterator pos, Args&&... args) {
    size_type index = pos - cbegin();
    
    if (size_ == capacity_) {
        size_type new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    // Move elements to make space
    for (size_type i = size_; i > index; --i) {
        if (i == size_) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move(data_[i-1]));
        } else {
            data_[i] = std::move(data_[i-1]);
        }
    }
    
    // Construct new element in place
    if (index < size_) {
        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + index);
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + index, std::forward<Args>(args)...);
    } else {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + index, std::forward<Args>(args)...);
    }
    
    ++size_;
    return iterator(data_ + index);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator pos) {
    size_type index = pos - cbegin();
    
    // Move elements to fill the gap
    for (size_type i = index; i < size_ - 1; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }
    
    // Destroy the last element
    std::allocator_traits<allocator_type>::destroy(alloc_, data_ + size_ - 1);
    --size_;
    
    return iterator(data_ + index);
}

template<typename T>
typename Vector<T>::iterator Vector<T>::erase(const_iterator first, const_iterator last) {
    if (first == last) return iterator(const_cast<T*>(&(*first)));
    
    size_type start_index = first - cbegin();
    size_type end_index = last - cbegin();
    size_type count = end_index - start_index;
    
    // Move elements to fill the gap
    for (size_type i = start_index; i < size_ - count; ++i) {
        data_[i] = std::move(data_[i + count]);
    }
    
    // Destroy the elements at the end
    for (size_type i = size_ - count; i < size_; ++i) {
        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
    }
    
    size_ -= count;
    return iterator(data_ + start_index);
}

template<typename T>
void Vector<T>::push_back(const T& value) {
    if (size_ == capacity_) {
        size_type new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, value);
    ++size_;
}

template<typename T>
void Vector<T>::push_back(T&& value) {
    if (size_ == capacity_) {
        size_type new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, std::move(value));
    ++size_;
}

template<typename T>
template<class... Args>
typename Vector<T>::reference Vector<T>::emplace_back(Args&&... args) {
    if (size_ == capacity_) {
        size_type new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate(new_capacity);
    }
    
    std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
    ++size_;
    
    return data_[size_ - 1];
}

template<typename T>
void Vector<T>::pop_back() {
    if (size_ > 0) {
        --size_;
        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + size_);
    }
}

template<typename T>
void Vector<T>::resize(size_type count) {
    if (count > size_) {
        if (count > capacity_) {
            reallocate(count);
        }
        
        for (size_type i = size_; i < count; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i);
        }
    } else if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
        }
    }
    
    size_ = count;
}

template<typename T>
void Vector<T>::resize(size_type count, const T& value) {
    if (count > size_) {
        if (count > capacity_) {
            reallocate(count);
        }
        
        for (size_type i = size_; i < count; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, value);
        }
    } else if (count < size_) {
        for (size_type i = count; i < size_; ++i) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
        }
    }
    
    size_ = count;
}

template<typename T>
void Vector<T>::swap(Vector& other) noexcept {
    if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
        std::swap(alloc_, other.alloc_);
    }
    
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

// Non-member functions
template<class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

template<class T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(rhs < lhs);
}

template<class T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    return rhs < lhs;
}

template<class T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs < rhs);
}

template<class T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

#endif // VECTOR_CPP