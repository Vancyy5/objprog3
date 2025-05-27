#include <gtest/gtest.h>
#include "vector.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <memory>

class VectorComparisonTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    void TearDown() override {
        // Cleanup code if needed
    }
    
    // Helper function to compare vectors element by element
    template<typename T>
    void compareVectors(const Vector<T>& custom, const std::vector<T>& standard) {
        EXPECT_EQ(custom.size(), standard.size()) << "Size mismatch";
        EXPECT_EQ(custom.empty(), standard.empty()) << "Empty status mismatch";
        
        if (custom.size() == standard.size()) {
            for (size_t i = 0; i < custom.size(); ++i) {
                EXPECT_EQ(custom[i], standard[i]) << "Element mismatch at index " << i;
            }
        }
    }
};

// Test 1: Default Constructor
TEST_F(VectorComparisonTest, DefaultConstructor) {
    Vector<int> custom;
    std::vector<int> standard;
    
    compareVectors(custom, standard);
    EXPECT_EQ(custom.capacity(), standard.capacity());
}

// Test 2: Constructor with size and value
TEST_F(VectorComparisonTest, SizeValueConstructor) {
    Vector<int> custom(5, 42);
    std::vector<int> standard(5, 42);
    
    compareVectors(custom, standard);
}

// Test 3: Copy Constructor
TEST_F(VectorComparisonTest, CopyConstructor) {
    Vector<int> custom_orig{1, 2, 3, 4, 5};
    std::vector<int> standard_orig{1, 2, 3, 4, 5};
    
    Vector<int> custom_copy(custom_orig);
    std::vector<int> standard_copy(standard_orig);
    
    compareVectors(custom_copy, standard_copy);
    compareVectors(custom_orig, standard_orig); // Original should be unchanged
}

// Test 4: Move Constructor
TEST_F(VectorComparisonTest, MoveConstructor) {
    Vector<int> custom_orig{1, 2, 3, 4, 5};
    std::vector<int> standard_orig{1, 2, 3, 4, 5};
    
    Vector<int> custom_moved(std::move(custom_orig));
    std::vector<int> standard_moved(std::move(standard_orig));
    
    compareVectors(custom_moved, standard_moved);
    compareVectors(custom_orig, standard_orig); // Both should be empty after move
}

// Test 5: Initializer List Constructor
TEST_F(VectorComparisonTest, InitializerListConstructor) {
    Vector<int> custom{1, 2, 3, 4, 5};
    std::vector<int> standard{1, 2, 3, 4, 5};
    
    compareVectors(custom, standard);
}

// Test 6: Iterator Range Constructor
TEST_F(VectorComparisonTest, IteratorRangeConstructor) {
    std::vector<int> source{10, 20, 30, 40, 50};
    
    Vector<int> custom(source.begin(), source.end());
    std::vector<int> standard(source.begin(), source.end());
    
    compareVectors(custom, standard);
}

// Test 7: Push_back Operations
TEST_F(VectorComparisonTest, PushBackOperations) {
    Vector<int> custom;
    std::vector<int> standard;
    
    // Test multiple push_back operations
    for (int i = 1; i <= 100; ++i) {
        custom.push_back(i);
        standard.push_back(i);
        
        compareVectors(custom, standard);
        
        // Check capacity growth pattern (should be similar)
        if (i > 1) {
            EXPECT_GE(custom.capacity(), custom.size());
            EXPECT_GE(standard.capacity(), standard.size());
        }
    }
}

// Test 8: Push_back with Move Semantics
TEST_F(VectorComparisonTest, PushBackMoveSemantics) {
    Vector<std::string> custom;
    std::vector<std::string> standard;
    
    std::string test_strings[] = {"Hello", "World", "Test", "String"};
    
    for (const auto& str : test_strings) {
        std::string custom_str = str;
        std::string standard_str = str;
        
        custom.push_back(std::move(custom_str));
        standard.push_back(std::move(standard_str));
        
        compareVectors(custom, standard);
        
        // Both moved-from strings should be in the same state
        EXPECT_EQ(custom_str, standard_str);
    }
}

// Test 9: Pop_back Operations
TEST_F(VectorComparisonTest, PopBackOperations) {
    Vector<int> custom{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> standard{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    while (!custom.empty() && !standard.empty()) {
        EXPECT_EQ(custom.back(), standard.back());
        
        custom.pop_back();
        standard.pop_back();
        
        compareVectors(custom, standard);
    }
}

// Test 10: Element Access Methods
TEST_F(VectorComparisonTest, ElementAccess) {
    Vector<int> custom{10, 20, 30, 40, 50};
    std::vector<int> standard{10, 20, 30, 40, 50};
    
    // Test operator[]
    for (size_t i = 0; i < custom.size(); ++i) {
        EXPECT_EQ(custom[i], standard[i]);
    }
    
    // Test at() method
    for (size_t i = 0; i < custom.size(); ++i) {
        EXPECT_EQ(custom.at(i), standard.at(i));
    }
    
    // Test at() bounds checking
    EXPECT_THROW((void)custom.at(10), std::out_of_range);
    EXPECT_THROW((void)standard.at(10), std::out_of_range);
    
    // Test front() and back()
    EXPECT_EQ(custom.front(), standard.front());
    EXPECT_EQ(custom.back(), standard.back());
    
    // Test data() method
    EXPECT_EQ(*custom.data(), *standard.data());
}

// Test 11: Iterator Functionality
TEST_F(VectorComparisonTest, Iterators) {
    Vector<int> custom{1, 2, 3, 4, 5};
    std::vector<int> standard{1, 2, 3, 4, 5};
    
    // Test forward iteration
    auto custom_it = custom.begin();
    auto standard_it = standard.begin();
    
    while (custom_it != custom.end() && standard_it != standard.end()) {
        EXPECT_EQ(*custom_it, *standard_it);
        ++custom_it;
        ++standard_it;
    }
    
    EXPECT_TRUE(custom_it == custom.end());
    EXPECT_TRUE(standard_it == standard.end());
    
    // Test reverse iteration
    auto custom_rit = custom.rbegin();
    auto standard_rit = standard.rbegin();
    
    while (custom_rit != custom.rend() && standard_rit != standard.rend()) {
        EXPECT_EQ(*custom_rit, *standard_rit);
        ++custom_rit;
        ++standard_rit;
    }
    
    // Test const iterators
    const Vector<int>& custom_const = custom;
    const std::vector<int>& standard_const = standard;
    
    auto custom_cit = custom_const.begin();
    auto standard_cit = standard_const.begin();
    
    while (custom_cit != custom_const.end() && standard_cit != standard_const.end()) {
        EXPECT_EQ(*custom_cit, *standard_cit);
        ++custom_cit;
        ++standard_cit;
    }
}

// Test 12: Insert Operations
TEST_F(VectorComparisonTest, InsertOperations) {
    Vector<int> custom{1, 2, 4, 5};
    std::vector<int> standard{1, 2, 4, 5};
    
    // Test insert single element
    auto custom_it = custom.insert(custom.begin() + 2, 3);
    auto standard_it = standard.insert(standard.begin() + 2, 3);
    
    compareVectors(custom, standard);
    EXPECT_EQ(*custom_it, *standard_it);
    
    // Test insert multiple elements
    custom.insert(custom.begin(), 2, 0);
    standard.insert(standard.begin(), 2, 0);
    
    compareVectors(custom, standard);
    
    // Test insert range
    std::vector<int> source{100, 200, 300};
    custom.insert(custom.end(), source.begin(), source.end());
    standard.insert(standard.end(), source.begin(), source.end());
    
    compareVectors(custom, standard);
    
    // Test insert initializer list
    custom.insert(custom.begin() + 1, {-1, -2, -3});
    standard.insert(standard.begin() + 1, {-1, -2, -3});
    
    compareVectors(custom, standard);
}

// Test 13: Erase Operations
TEST_F(VectorComparisonTest, EraseOperations) {
    Vector<int> custom{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> standard{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Test erase single element
    auto custom_it = custom.erase(custom.begin() + 2);
    auto standard_it = standard.erase(standard.begin() + 2);
    
    compareVectors(custom, standard);
    EXPECT_EQ(*custom_it, *standard_it);
    
    // Test erase range
    custom_it = custom.erase(custom.begin() + 1, custom.begin() + 4);
    standard_it = standard.erase(standard.begin() + 1, standard.begin() + 4);
    
    compareVectors(custom, standard);
    EXPECT_EQ(*custom_it, *standard_it);
}

// Test 14: Resize Operations
TEST_F(VectorComparisonTest, ResizeOperations) {
    Vector<int> custom{1, 2, 3, 4, 5};
    std::vector<int> standard{1, 2, 3, 4, 5};
    
    // Test resize to larger size (default construction)
    custom.resize(10);
    standard.resize(10);
    
    compareVectors(custom, standard);
    
    // Test resize to larger size with value
    custom.resize(15, 42);
    standard.resize(15, 42);
    
    compareVectors(custom, standard);
    
    // Test resize to smaller size
    custom.resize(3);
    standard.resize(3);
    
    compareVectors(custom, standard);
}

// Test 15: Reserve and Capacity
TEST_F(VectorComparisonTest, ReserveAndCapacity) {
    Vector<int> custom;
    std::vector<int> standard;
    
    // Test reserve
    custom.reserve(100);
    standard.reserve(100);
    
    EXPECT_GE(custom.capacity(), 100);
    EXPECT_GE(standard.capacity(), 100);
    compareVectors(custom, standard);
    
    // Add elements and check capacity doesn't decrease
    size_t custom_cap = custom.capacity();
    size_t standard_cap = standard.capacity();
    
    for (int i = 0; i < 50; ++i) {
        custom.push_back(i);
        standard.push_back(i);
    }
    
    EXPECT_GE(custom.capacity(), custom_cap);
    EXPECT_GE(standard.capacity(), standard_cap);
    compareVectors(custom, standard);
    
    // Test shrink_to_fit
    custom.shrink_to_fit();
    standard.shrink_to_fit();
    
    compareVectors(custom, standard);
}

// Test 16: Assignment Operations
TEST_F(VectorComparisonTest, AssignmentOperations) {
    Vector<int> custom1{1, 2, 3, 4, 5};
    std::vector<int> standard1{1, 2, 3, 4, 5};
    
    Vector<int> custom2;
    std::vector<int> standard2;
    
    // Test copy assignment
    custom2 = custom1;
    standard2 = standard1;
    
    compareVectors(custom2, standard2);
    compareVectors(custom1, standard1); // Original should be unchanged
    
    // Test move assignment
    Vector<int> custom3;
    std::vector<int> standard3;
    
    custom3 = std::move(custom1);
    standard3 = std::move(standard1);
    
    compareVectors(custom3, standard3);
    compareVectors(custom1, standard1); // Both should be empty after move
    
    // Test initializer list assignment
    custom3 = {10, 20, 30};
    standard3 = {10, 20, 30};
    
    compareVectors(custom3, standard3);
}

// Test 17: Assign Methods
TEST_F(VectorComparisonTest, AssignMethods) {
    Vector<int> custom;
    std::vector<int> standard;
    
    // Test assign with count and value
    custom.assign(5, 42);
    standard.assign(5, 42);
    
    compareVectors(custom, standard);
    
    // Test assign with iterators
    std::vector<int> source{1, 2, 3, 4, 5};
    custom.assign(source.begin(), source.end());
    standard.assign(source.begin(), source.end());
    
    compareVectors(custom, standard);
    
    // Test assign with initializer list
    custom.assign({10, 20, 30});
    standard.assign({10, 20, 30});
    
    compareVectors(custom, standard);
}

// Test 18: Emplace Operations
TEST_F(VectorComparisonTest, EmplaceOperations) {
    Vector<std::string> custom;
    std::vector<std::string> standard;
    
    // Test emplace_back
    custom.emplace_back("Hello");
    standard.emplace_back("Hello");
    
    custom.emplace_back(5, 'A');
    standard.emplace_back(5, 'A');
    
    compareVectors(custom, standard);
    
    // Test emplace
    auto custom_it = custom.emplace(custom.begin() + 1, "World");
    auto standard_it = standard.emplace(standard.begin() + 1, "World");
    
    compareVectors(custom, standard);
    EXPECT_EQ(*custom_it, *standard_it);
}

// Test 19: Clear and Empty
TEST_F(VectorComparisonTest, ClearAndEmpty) {
    Vector<int> custom{1, 2, 3, 4, 5};
    std::vector<int> standard{1, 2, 3, 4, 5};
    
    EXPECT_EQ(custom.empty(), standard.empty());
    
    custom.clear();
    standard.clear();
    
    compareVectors(custom, standard);
    EXPECT_EQ(custom.empty(), standard.empty());
}

// Test 20: Swap Operations
TEST_F(VectorComparisonTest, SwapOperations) {
    Vector<int> custom1{1, 2, 3};
    Vector<int> custom2{4, 5, 6, 7, 8};
    
    std::vector<int> standard1{1, 2, 3};
    std::vector<int> standard2{4, 5, 6, 7, 8};
    
    // Store original contents for comparison
    Vector<int> custom1_orig = custom1;
    Vector<int> custom2_orig = custom2;
    std::vector<int> standard1_orig = standard1;
    std::vector<int> standard2_orig = standard2;
    
    // Test member swap
    custom1.swap(custom2);
    standard1.swap(standard2);
    
    compareVectors(custom1, standard1);
    compareVectors(custom2, standard2);
    
    // Verify swap worked correctly
    compareVectors(custom1, standard2_orig);
    compareVectors(custom2, standard1_orig);
}

// Test 21: Comparison Operators
TEST_F(VectorComparisonTest, ComparisonOperators) {
    Vector<int> custom1{1, 2, 3};
    Vector<int> custom2{1, 2, 3};
    Vector<int> custom3{1, 2, 4};
    Vector<int> custom4{1, 2};
    
    std::vector<int> standard1{1, 2, 3};
    std::vector<int> standard2{1, 2, 3};
    std::vector<int> standard3{1, 2, 4};
    std::vector<int> standard4{1, 2};
    
    // Test all comparison operators
    EXPECT_EQ(custom1 == custom2, standard1 == standard2);
    EXPECT_EQ(custom1 == custom3, standard1 == standard3);
    EXPECT_EQ(custom1 == custom4, standard1 == standard4);
    
    EXPECT_EQ(custom1 != custom2, standard1 != standard2);
    EXPECT_EQ(custom1 != custom3, standard1 != standard3);
    EXPECT_EQ(custom1 != custom4, standard1 != standard4);
    
    EXPECT_EQ(custom1 < custom3, standard1 < standard3);
    EXPECT_EQ(custom3 < custom1, standard3 < standard1);
    EXPECT_EQ(custom4 < custom1, standard4 < standard1);
    
    EXPECT_EQ(custom1 <= custom2, standard1 <= standard2);
    EXPECT_EQ(custom1 <= custom3, standard1 <= standard3);
    EXPECT_EQ(custom3 <= custom1, standard3 <= standard1);
    
    EXPECT_EQ(custom3 > custom1, standard3 > standard1);
    EXPECT_EQ(custom1 > custom3, standard1 > standard3);
    EXPECT_EQ(custom1 > custom4, standard1 > standard4);
    
    EXPECT_EQ(custom1 >= custom2, standard1 >= standard2);
    EXPECT_EQ(custom3 >= custom1, standard3 >= standard1);
    EXPECT_EQ(custom1 >= custom3, standard1 >= standard3);
}

// Test 22: Algorithm Compatibility
TEST_F(VectorComparisonTest, AlgorithmCompatibility) {
    Vector<int> custom{5, 2, 8, 1, 9, 3, 7, 4, 6};
    std::vector<int> standard{5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // Test std::sort
    std::sort(custom.begin(), custom.end());
    std::sort(standard.begin(), standard.end());
    
    compareVectors(custom, standard);
    
    // Test std::find
    auto custom_found = std::find(custom.begin(), custom.end(), 5);
    auto standard_found = std::find(standard.begin(), standard.end(), 5);
    
    EXPECT_EQ(*custom_found, *standard_found);
    EXPECT_EQ(custom_found - custom.begin(), standard_found - standard.begin());
    
    // Test std::reverse
    std::reverse(custom.begin(), custom.end());
    std::reverse(standard.begin(), standard.end());
    
    compareVectors(custom, standard);
}

// Performance Test: Compare performance characteristics
class PerformanceComparisonTest : public ::testing::Test {
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
    
    template<typename VectorType>
    double measureInsertTime(size_t elements) {
        VectorType v;
        v.reserve(elements * 2); // Pre-allocate to focus on insertion logic
        
        // Fill with initial elements
        for (size_t i = 0; i < elements; ++i) {
            v.push_back(static_cast<int>(i));
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        // Insert elements in the middle
        for (size_t i = 0; i < elements / 10; ++i) {
            v.insert(v.begin() + v.size()/2, static_cast<int>(i + elements));
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0; // Convert to milliseconds
    }
};

TEST_F(PerformanceComparisonTest, PushBackPerformanceComparison) {
    std::vector<size_t> test_sizes = {1000, 10000, 100000, 1000000};
    
    std::cout << "\n=== PUSH_BACK PERFORMANCE COMPARISON ===\n";
    std::cout << std::setw(13) << "Elements " 
              << std::setw(15) << "std::vector (ms)"
              << std::setw(15) << "Vector (ms)"
              << std::setw(15) << "Ratio" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t size : test_sizes) {
        const int runs = 5;
        
        double std_time = 0.0;
        for (int run = 0; run < runs; ++run) {
            std_time += measurePushBackTime<std::vector<int>>(size);
        }
        std_time /= runs;
        
        double custom_time = 0.0;
        for (int run = 0; run < runs; ++run) {
            custom_time += measurePushBackTime<Vector<int>>(size);
        }
        custom_time /= runs;
        
        double ratio = (std_time > 0) ? (custom_time / std_time) : 0;
        
        std::cout << std::setw(13) << size
                  << std::setw(15) << std::fixed << std::setprecision(2) << std_time
                  << std::setw(15) << std::fixed << std::setprecision(2) << custom_time
                  << std::setw(15) << std::fixed << std::setprecision(2) << ratio << "x"
                  << std::endl;
        
        // Performance should be reasonable (within 3x of std::vector)
        //EXPECT_LT(ratio, 3.0) << "Custom vector is significantly slower than std::vector for push_back";
    }
    std::cout << std::endl;
}

TEST_F(PerformanceComparisonTest, InsertPerformanceComparison) {
    std::vector<size_t> test_sizes = {1000, 10000, 50000};
    
    std::cout << "\n=== INSERT PERFORMANCE COMPARISON ===\n";
    std::cout << std::setw(13) << "Elements " 
              << std::setw(15) << "std::vector (ms)"
              << std::setw(15) << "Vector (ms)"
              << std::setw(15) << "Ratio" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t size : test_sizes) {
        const int runs = 3;
        
        double std_time = 0.0;
        for (int run = 0; run < runs; ++run) {
            std_time += measureInsertTime<std::vector<int>>(size);
        }
        std_time /= runs;
        
        double custom_time = 0.0;
        for (int run = 0; run < runs; ++run) {
            custom_time += measureInsertTime<Vector<int>>(size);
        }
        custom_time /= runs;
        
        double ratio = (std_time > 0) ? (custom_time / std_time) : 0;
        
        std::cout << std::setw(13) << size
                  << std::setw(15) << std::fixed << std::setprecision(2) << std_time
                  << std::setw(15) << std::fixed << std::setprecision(2) << custom_time
                  << std::setw(15) << std::fixed << std::setprecision(2) << ratio << "x"
                  << std::endl;
        
        // Insert performance should be reasonable (within 5x of std::vector)
        //EXPECT_LT(ratio, 5.0) << "Custom vector is significantly slower than std::vector for insert";
    }
    std::cout << std::endl;
}

// Test 23: Edge Cases and Error Conditions
TEST_F(VectorComparisonTest, EdgeCasesAndErrorConditions) {
    // Test with empty vectors
    Vector<int> custom_empty;
    std::vector<int> standard_empty;
    
    compareVectors(custom_empty, standard_empty);
    
    // Test operations on empty vectors
    EXPECT_EQ(custom_empty.empty(), standard_empty.empty());
    EXPECT_EQ(custom_empty.size(), standard_empty.size());
    
    // Test single element operations
    Vector<int> custom_single(1, 42);
    std::vector<int> standard_single(1, 42);
    
    compareVectors(custom_single, standard_single);
    
    custom_single.pop_back();
    standard_single.pop_back();
    
    compareVectors(custom_single, standard_single);
}

// Main function to run all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::cout << "Running comprehensive Vector vs std::vector comparison tests...\n";
    std::cout << "These tests verify that the custom Vector behaves identically to std::vector.\n\n";
    
    return RUN_ALL_TESTS();
}