#include <gtest/gtest.h>
#include "vector.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>

class VectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
};

// Test 1: Constructors and Basic Operations
TEST_F(VectorTest, ConstructorsAndBasicOperations) {
    // Default constructor
    Vector<int> v1;
    EXPECT_TRUE(v1.empty());
    EXPECT_EQ(v1.size(), 0);
    
    // Constructor with size and value
    Vector<int> v2(5, 42);
    EXPECT_EQ(v2.size(), 5);
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 42);
    }
    
    // Copy constructor
    Vector<int> v3(v2);
    EXPECT_EQ(v3.size(), v2.size());
    for (size_t i = 0; i < v3.size(); ++i) {
        EXPECT_EQ(v3[i], v2[i]);
    }
    
    // Move constructor
    Vector<int> v4(std::move(v3));
    EXPECT_EQ(v4.size(), 5);
    EXPECT_TRUE(v3.empty()); // v3 should be empty after move
    
    // Initializer list constructor
    Vector<int> v5{1, 2, 3, 4, 5};
    EXPECT_EQ(v5.size(), 5);
    for (size_t i = 0; i < v5.size(); ++i) {
        EXPECT_EQ(v5[i], static_cast<int>(i + 1));
    }
}

// Test 2: Push_back and Pop_back
TEST_F(VectorTest, PushBackPopBack) {
    Vector<int> v;
    
    // Test push_back
    for (int i = 1; i <= 10; ++i) {
        v.push_back(i);
        EXPECT_EQ(v.size(), static_cast<size_t>(i));
        EXPECT_EQ(v.back(), i);
    }
    
    // Test pop_back
    for (int i = 10; i >= 1; --i) {
        EXPECT_EQ(v.back(), i);
        v.pop_back();
        EXPECT_EQ(v.size(), static_cast<size_t>(i - 1));
    }
    
    EXPECT_TRUE(v.empty());
}

// Test 3: Element Access
TEST_F(VectorTest, ElementAccess) {
    Vector<int> v{10, 20, 30, 40, 50};
    
    // Test operator[]
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], static_cast<int>((i + 1) * 10));
    }
    
    // Test at() method
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v.at(i), static_cast<int>((i + 1) * 10));
    }
    
    // Test at() bounds checking
    EXPECT_THROW(v.at(10), std::out_of_range);
    
    // Test front() and back()
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 50);
    
    // Test modification through operator[]
    v[0] = 100;
    EXPECT_EQ(v[0], 100);
}

// Test 4: Iterators
TEST_F(VectorTest, Iterators) {
    Vector<int> v{1, 2, 3, 4, 5};
    
    // Test begin() and end()
    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    // Test const iterators
    const Vector<int>& cv = v;
    expected = 1;
    for (auto it = cv.begin(); it != cv.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    // Test range-based for loop
    expected = 1;
    for (const auto& element : v) {
        EXPECT_EQ(element, expected++);
    }
    
    // Test iterator arithmetic
    auto it = v.begin();
    EXPECT_EQ(*(it + 2), 3);
    EXPECT_EQ(*(it + 4), 5);
    
    // Test reverse iterators
    expected = 5;
    for (auto rit = v.rbegin(); rit != v.rend(); ++rit) {
        EXPECT_EQ(*rit, expected--);
    }
}

// Test 5: Insert and Erase
TEST_F(VectorTest, InsertErase) {
    Vector<int> v{1, 2, 4, 5};
    
    // Test insert single element
    auto it = v.insert(v.begin() + 2, 3);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(*it, 3);
    
    // Verify order
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
    
    // Test insert multiple elements
    v.insert(v.begin(), 2, 0);
    EXPECT_EQ(v.size(), 7);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 0);
    
    // Test erase single element
    it = v.erase(v.begin());
    EXPECT_EQ(v.size(), 6);
    EXPECT_EQ(*it, 0);
    
    // Test erase range
    it = v.erase(v.begin(), v.begin() + 2);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 2);
}

// Test 6: Capacity Management
TEST_F(VectorTest, CapacityManagement) {
    Vector<int> v;
    
    // Test reserve
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100);
    EXPECT_EQ(v.size(), 0);
    
    // Test that capacity doesn't decrease when adding elements
    size_t initial_capacity = v.capacity();
    for (int i = 0; i < 50; ++i) {
        v.push_back(i);
    }
    EXPECT_GE(v.capacity(), initial_capacity);
    
    // Test resize
    v.resize(10);
    EXPECT_EQ(v.size(), 10);
    
    v.resize(20, 42);
    EXPECT_EQ(v.size(), 20);
    for (size_t i = 10; i < 20; ++i) {
        EXPECT_EQ(v[i], 42);
    }
    
    // Test shrink_to_fit
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
    
    // Test clear
    v.clear();
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
}

// Test 7: Assignment Operators
TEST_F(VectorTest, AssignmentOperators) {
    Vector<int> v1{1, 2, 3, 4, 5};
    Vector<int> v2;
    
    // Test copy assignment
    v2 = v1;
    EXPECT_EQ(v2.size(), v1.size());
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], v1[i]);
    }
    
    // Test move assignment
    Vector<int> v3;
    v3 = std::move(v1);
    EXPECT_EQ(v3.size(), 5);
    EXPECT_TRUE(v1.empty());
    
    // Test initializer list assignment
    v3 = {10, 20, 30};
    EXPECT_EQ(v3.size(), 3);
    EXPECT_EQ(v3[0], 10);
    EXPECT_EQ(v3[1], 20);
    EXPECT_EQ(v3[2], 30);
}

// Test 8: Comparison Operators
TEST_F(VectorTest, ComparisonOperators) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 2, 3};
    Vector<int> v3{1, 2, 4};
    Vector<int> v4{1, 2};
    
    // Test equality
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 == v4);
    
    // Test inequality
    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);
    EXPECT_TRUE(v1 != v4);
    
    // Test less than
    EXPECT_TRUE(v1 < v3);
    EXPECT_FALSE(v3 < v1);
    EXPECT_TRUE(v4 < v1);
    
    // Test other comparison operators
    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v1 <= v3);
    EXPECT_FALSE(v3 <= v1);
    
    EXPECT_TRUE(v3 > v1);
    EXPECT_FALSE(v1 > v3);
    EXPECT_TRUE(v1 > v4);
    
    EXPECT_TRUE(v1 >= v2);
    EXPECT_TRUE(v3 >= v1);
    EXPECT_FALSE(v1 >= v3);
}

// Test 9: Emplace Methods
TEST_F(VectorTest, EmplaceMethods) {
    Vector<std::string> v;
    
    // Test emplace_back
    v.emplace_back("Hello");
    v.emplace_back(5, 'A'); // Constructor with count and character
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "Hello");
    EXPECT_EQ(v[1], "AAAAA");
    
    // Test emplace
    auto it = v.emplace(v.begin() + 1, "World");
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(*it, "World");
    EXPECT_EQ(v[1], "World");
}

// Test 10: Swap
TEST_F(VectorTest, Swap) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{4, 5, 6, 7, 8};
    
    size_t v1_size = v1.size();
    size_t v2_size = v2.size();
    
    v1.swap(v2);
    
    EXPECT_EQ(v1.size(), v2_size);
    EXPECT_EQ(v2.size(), v1_size);
    
    EXPECT_EQ(v1[0], 4);
    EXPECT_EQ(v2[0], 1);
}

// Performance Test: Push_back Performance Comparison
class PerformanceTest : public ::testing::Test {
protected:
    template<typename VectorType>
    double measurePushBackTime(size_t elements) {
        auto start = std::chrono::high_resolution_clock::now();
        
        VectorType v;
        for (size_t i = 0; i < elements; ++i) {
            v.push_back(static_cast<int>(i));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0; // Convert to milliseconds
    }
};

TEST_F(PerformanceTest, PushBackPerformanceComparison) {
    std::vector<size_t> test_sizes = {10000, 100000, 1000000, 10000000, 100000000};
    
    std::cout << "\n=== PUSH_BACK PERFORMANCE COMPARISON ===\n";
    std::cout << std::setw(12) << "Elements" 
              << std::setw(15) << "std::vector (ms)"
              << std::setw(15) << "Vector (ms)"
              << std::setw(15) << "Ratio" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t size : test_sizes) {
        // Measure std::vector performance
        double std_time = 0.0;
        const int runs = (size <= 1000000) ? 5 : 1; // Multiple runs for smaller sizes
        
        for (int run = 0; run < runs; ++run) {
            std_time += measurePushBackTime<std::vector<int>>(size);
        }
        std_time /= runs;
        
        // Measure custom Vector performance
        double custom_time = 0.0;
        for (int run = 0; run < runs; ++run) {
            custom_time += measurePushBackTime<Vector<int>>(size);
        }
        custom_time /= runs;
        
        double ratio = (std_time > 0) ? (custom_time / std_time) : 0;
        
        std::cout << std::setw(12) << size
                  << std::setw(15) << std::fixed << std::setprecision(2) << std_time
                  << std::setw(15) << std::fixed << std::setprecision(2) << custom_time
                  << std::setw(15) << std::fixed << std::setprecision(2) << ratio << "x"
                  << std::endl;
    }
    std::cout << std::endl;
}

// Additional functionality tests
TEST_F(VectorTest, AssignMethods) {
    Vector<int> v;
    
    // Test assign with count and value
    v.assign(5, 42);
    EXPECT_EQ(v.size(), 5);
    for (const auto& elem : v) {
        EXPECT_EQ(elem, 42);
    }
    
    // Test assign with iterators
    std::vector<int> source{1, 2, 3, 4, 5};
    v.assign(source.begin(), source.end());
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], source[i]);
    }
    
    // Test assign with initializer list
    v.assign({10, 20, 30});
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST_F(VectorTest, DataAccess) {
    Vector<int> v{1, 2, 3, 4, 5};
    
    // Test data() method
    int* ptr = v.data();
    EXPECT_EQ(*ptr, 1);
    EXPECT_EQ(*(ptr + 1), 2);
    
    // Test const data() method
    const Vector<int>& cv = v;
    const int* const_ptr = cv.data();
    EXPECT_EQ(*const_ptr, 1);
    EXPECT_EQ(*(const_ptr + 1), 2);
}

// Main function to run all tests
int main(int argc, char** argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}