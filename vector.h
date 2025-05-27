#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <type_traits>

template<typename T>
class Vector {
public:
    // Member types
    using value_type = T;
    using allocator_type = std::allocator<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
    
    // Iterator classes
    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        iterator() : ptr_(nullptr) {}
        explicit iterator(pointer ptr) : ptr_(ptr) {}
        
        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }
        reference operator[](difference_type n) const { return ptr_[n]; }
        
        iterator& operator++() { ++ptr_; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++ptr_; return tmp; }
        iterator& operator--() { --ptr_; return *this; }
        iterator operator--(int) { iterator tmp = *this; --ptr_; return tmp; }
        
        iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
        iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }
        iterator operator+(difference_type n) const { return iterator(ptr_ + n); }
        iterator operator-(difference_type n) const { return iterator(ptr_ - n); }
        difference_type operator-(const iterator& other) const { return ptr_ - other.ptr_; }
        
        bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
        bool operator<(const iterator& other) const { return ptr_ < other.ptr_; }
        bool operator<=(const iterator& other) const { return ptr_ <= other.ptr_; }
        bool operator>(const iterator& other) const { return ptr_ > other.ptr_; }
        bool operator>=(const iterator& other) const { return ptr_ >= other.ptr_; }
        
        friend iterator operator+(difference_type n, const iterator& it) { return it + n; }
        
    private:
        pointer ptr_;
    };
    
    class const_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        
        const_iterator() : ptr_(nullptr) {}
        explicit const_iterator(pointer ptr) : ptr_(ptr) {}
        const_iterator(const iterator& it) : ptr_(&(*it)) {}
        
        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }
        reference operator[](difference_type n) const { return ptr_[n]; }
        
        const_iterator& operator++() { ++ptr_; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++ptr_; return tmp; }
        const_iterator& operator--() { --ptr_; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --ptr_; return tmp; }
        
        const_iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
        const_iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }
        const_iterator operator+(difference_type n) const { return const_iterator(ptr_ + n); }
        const_iterator operator-(difference_type n) const { return const_iterator(ptr_ - n); }
        difference_type operator-(const const_iterator& other) const { return ptr_ - other.ptr_; }
        
        bool operator==(const const_iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const const_iterator& other) const { return ptr_ != other.ptr_; }
        bool operator<(const const_iterator& other) const { return ptr_ < other.ptr_; }
        bool operator<=(const const_iterator& other) const { return ptr_ <= other.ptr_; }
        bool operator>(const const_iterator& other) const { return ptr_ > other.ptr_; }
        bool operator>=(const const_iterator& other) const { return ptr_ >= other.ptr_; }
        
        friend const_iterator operator+(difference_type n, const const_iterator& it) { return it + n; }
        
    private:
        pointer ptr_;
    };
    
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    T* data_;
    size_type size_;
    size_type capacity_;
    allocator_type alloc_;

    void reallocate(size_type new_capacity);
    void destroy_elements();

public:
    // Constructors and destructor
    Vector();
    explicit Vector(const allocator_type& alloc);
    explicit Vector(size_type count, const T& value = T(), const allocator_type& alloc = allocator_type());
    
    // Iterator constructor with proper SFINAE
    template<class InputIt>
    Vector(InputIt first, InputIt last, const allocator_type& alloc = allocator_type(),
           typename std::enable_if_t<!std::is_integral_v<InputIt>>* = nullptr);
    
    Vector(const Vector& other);
    Vector(const Vector& other, const allocator_type& alloc);
    Vector(Vector&& other) noexcept;
    Vector(Vector&& other, const allocator_type& alloc);
    Vector(std::initializer_list<T> init, const allocator_type& alloc = allocator_type());
    ~Vector();

    // Assignment operators
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    Vector& operator=(std::initializer_list<T> ilist);

    // assign methods
    void assign(size_type count, const T& value);
    template<class InputIt>
    void assign(InputIt first, InputIt last,
                typename std::enable_if_t<!std::is_integral_v<InputIt>>* = nullptr);
    void assign(std::initializer_list<T> ilist);

    // get_allocator
    allocator_type get_allocator() const { return alloc_; }

    // Element access
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }
    reference front() { return data_[0]; }
    const_reference front() const { return data_[0]; }
    reference back() { return data_[size_ - 1]; }
    const_reference back() const { return data_[size_ - 1]; }
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }

    // Iterators
    iterator begin() noexcept { return iterator(data_); }
    const_iterator begin() const noexcept { return const_iterator(data_); }
    const_iterator cbegin() const noexcept { return const_iterator(data_); }
    iterator end() noexcept { return iterator(data_ + size_); }
    const_iterator end() const noexcept { return const_iterator(data_ + size_); }
    const_iterator cend() const noexcept { return const_iterator(data_ + size_); }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    // Capacity
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return std::allocator_traits<allocator_type>::max_size(alloc_); }
    void reserve(size_type new_cap);
    size_type capacity() const noexcept { return capacity_; }
    void shrink_to_fit();

    // Modifiers
    void clear() noexcept;
    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_type count, const T& value);
    template<class InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last,
                    typename std::enable_if_t<!std::is_integral_v<InputIt>>* = nullptr);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);
    
    template<class... Args>
    iterator emplace(const_iterator pos, Args&&... args);
    
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    
    void push_back(const T& value);
    void push_back(T&& value);
    
    template<class... Args>
    reference emplace_back(Args&&... args);
    
    void pop_back();
    
    void resize(size_type count);
    void resize(size_type count, const T& value);
    
    void swap(Vector& other) noexcept;
};

// Non-member functions
template<class T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs);

template<class T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs);

template<class T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs);

template<class T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs);

template<class T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs);

template<class T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs);

template<class T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept;

// Include implementation
#include "vector.cpp"

#endif // VECTOR_H