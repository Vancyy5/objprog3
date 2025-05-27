#include <gtest/gtest.h>
#include "vector.h"
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

class VectorTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
    
    // Helper to compare our Vector with std::vector
    template<typename T>
    void expectEqual(const Vector<T>& v1, const std::vector<T>& v2) {
        EXPECT_EQ(v1.size(), v2.size());
        EXPECT_EQ(v1.empty(), v2.empty());
        
        for (size_t i = 0; i < v1.size() && i < v2.size(); ++i) {
            EXPECT_EQ(v1[i], v2[i]) << "Elements differ at index " << i;
        }
    }
};

// Basic Constructor Tests
TEST_F(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.data(), nullptr);
}

TEST_F(VectorTest, CountValueConstructor) {
    Vector<int> v(5, 42);
    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 42);
    }
    
    // Test zero count
    Vector<int> v_zero(0, 10);
    EXPECT_TRUE(v_zero.empty());
    EXPECT_EQ(v_zero.size(), 0);
}

TEST_F(VectorTest, InitializerListConstructor) {
    Vector<int> v{1, 2, 3, 4, 5};
    EXPECT_EQ(v.size(), 5);
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], i + 1);
    }
    
    // Test empty initializer list
    Vector<int> v_empty{};
    EXPECT_TRUE(v_empty.empty());
}

TEST_F(VectorTest, CopyConstructor) {
    Vector<int> original{1, 2, 3, 4, 5};
    Vector<int> copy(original);
    
    EXPECT_EQ(copy.size(), original.size());
    for (size_t i = 0; i < copy.size(); ++i) {
        EXPECT_EQ(copy[i], original[i]);
    }
    
    // Modify copy to ensure independence
    copy[0] = 99;
    EXPECT_EQ(original[0], 1);
    EXPECT_EQ(copy[0], 99);
}

TEST_F(VectorTest, MoveConstructor) {
    Vector<int> original{1, 2, 3, 4, 5};
    size_t originalSize = original.size();
    
    Vector<int> moved(std::move(original));
    
    EXPECT_EQ(moved.size(), originalSize);
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.data(), nullptr);
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(moved[i], i + 1);
    }
}

// Assignment Operator Tests
TEST_F(VectorTest, CopyAssignment) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2;
    
    v2 = v1;
    
    EXPECT_EQ(v2.size(), v1.size());
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], v1[i]);
    }
    
    // Test self-assignment
    v1 = v1;
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1[0], 1);
}

TEST_F(VectorTest, MoveAssignment) {
    Vector<int> v1{1, 2, 3, 4, 5};
    Vector<int> v2;
    
    v2 = std::move(v1);
    
    EXPECT_EQ(v2.size(), 5);
    EXPECT_EQ(v1.size(), 0);
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(v2[i], i + 1);
    }
    
    // Test self-move-assignment
    Vector<int> v3{1, 2, 3};
    v3 = std::move(v3);
    EXPECT_EQ(v3.size(), 3);
    EXPECT_EQ(v3[0], 1);
}

TEST_F(VectorTest, InitializerListAssignment) {
    Vector<int> v;
    v = {10, 20, 30};
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

// Element Access Tests
TEST_F(VectorTest, ElementAccess) {
    Vector<int> v{10, 20, 30, 40, 50};
    
    // Test operator[]
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[4], 50);
    
    // Test at()
    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(4), 50);
    
    // Test bounds checking with at()
    EXPECT_THROW(v.at(5), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
    
    // Test front() and back()
    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 50);
    
    // Test data()
    EXPECT_EQ(*v.data(), 10);
    EXPECT_EQ(v.data()[1], 20);
    
    // Test const versions
    const Vector<int>& cv = v;
    EXPECT_EQ(cv.at(0), 10);
    EXPECT_EQ(cv[0], 10);
    EXPECT_EQ(cv.front(), 10);
    EXPECT_EQ(cv.back(), 50);
    EXPECT_EQ(*cv.data(), 10);
    
    // Test const at() bounds checking
    EXPECT_THROW(cv.at(5), std::out_of_range);
}

// Capacity Tests
TEST_F(VectorTest, CapacityOperations) {
    Vector<int> v;
    
    // Test empty vector
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    
    // Test reserve
    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);
    EXPECT_EQ(v.size(), 0);
    
    // Test reserve with smaller capacity (should not shrink)
    size_t currentCapacity = v.capacity();
    v.reserve(5);
    EXPECT_EQ(v.capacity(), currentCapacity);
    
    // Add elements
    for (int i = 0; i < 5; ++i) {
        v.push_back(i);
    }
    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 10);
    
    // Test shrink_to_fit
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());
}

// Modifier Tests
TEST_F(VectorTest, PushBackAndPopBack) {
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
    
    // Test pop_back on empty vector (should not crash)
    v.pop_back();
    EXPECT_TRUE(v.empty());
}

TEST_F(VectorTest, PushBackMoveSemantics) {
    Vector<std::string> v;
    
    std::string s1 = "Hello";
    std::string s2 = "World";
    
    v.push_back(std::move(s1));
    v.push_back(std::move(s2));
    
    EXPECT_EQ(v[0], "Hello");
    EXPECT_EQ(v[1], "World");
    EXPECT_EQ(v.size(), 2);
}

TEST_F(VectorTest, EmplaceBack) {
    Vector<std::string> v;
    
    // Emplace with constructor arguments
    v.emplace_back("Hello");
    v.emplace_back(5, 'A');  // "AAAAA"
    
    EXPECT_EQ(v[0], "Hello");
    EXPECT_EQ(v[1], "AAAAA");
    EXPECT_EQ(v.size(), 2);
    
    // Test return value
    std::string& ref = v.emplace_back("Test");
    EXPECT_EQ(ref, "Test");
    EXPECT_EQ(&ref, &v.back());
}

TEST_F(VectorTest, Clear) {
    Vector<int> v{1, 2, 3, 4, 5};
    size_t originalCapacity = v.capacity();
    
    EXPECT_EQ(v.size(), 5);
    EXPECT_FALSE(v.empty());
    
    v.clear();
    
    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
    // Capacity should remain unchanged
    EXPECT_EQ(v.capacity(), originalCapacity);
}

TEST_F(VectorTest, Resize) {
    Vector<int> v{1, 2, 3};
    
    // Resize to larger size (default construction)
    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 0);  // default constructed
    EXPECT_EQ(v[4], 0);
    
    // Resize to larger size with value
    v.resize(7, 42);
    EXPECT_EQ(v.size(), 7);
    EXPECT_EQ(v[5], 42);
    EXPECT_EQ(v[6], 42);
    
    // Resize to smaller size
    v.resize(2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    
    // Resize to zero
    v.resize(0);
    EXPECT_TRUE(v.empty());
}

// Insert Tests
TEST_F(VectorTest, InsertSingleElement) {
    Vector<int> v{1, 3, 5};
    
    // Insert at beginning
    auto it = v.insert(v.begin(), 0);
    EXPECT_EQ(*it, 0);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    
    // Insert in middle
    it = v.insert(v.begin() + 2, 2);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(v.size(), 5);
    // Should be: 0, 1, 2, 3, 5
    for (int i = 0; i < 5; ++i) {
        if (i == 4) {
            EXPECT_EQ(v[i], 5);
        } else {
            EXPECT_EQ(v[i], i);
        }
    }
    
    // Insert at end
    it = v.insert(v.end(), 6);
    EXPECT_EQ(*it, 6);
    EXPECT_EQ(v.back(), 6);
}

TEST_F(VectorTest, InsertMoveElement) {
    Vector<std::string> v{"Hello", "World"};
    
    std::string s = "Test";
    auto it = v.insert(v.begin() + 1, std::move(s));
    
    EXPECT_EQ(*it, "Test");
    EXPECT_EQ(v[0], "Hello");
    EXPECT_EQ(v[1], "Test");
    EXPECT_EQ(v[2], "World");
}

TEST_F(VectorTest, InsertMultipleElements) {
    Vector<int> v{1, 5};
    
    // Insert multiple elements
    auto it = v.insert(v.begin() + 1, 3, 2);
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(v.size(), 5);
    
    // Should be: 1, 2, 2, 2, 5
    EXPECT_EQ(v[0], 1);
    for (int i = 1; i < 4; ++i) {
        EXPECT_EQ(v[i], 2);
    }
    EXPECT_EQ(v[4], 5);
    
    // Test insert zero elements
    size_t sizeBefore = v.size();
    it = v.insert(v.begin(), 0, 99);
    EXPECT_EQ(v.size(), sizeBefore);
    EXPECT_EQ(it, v.begin());
}

// Erase Tests
TEST_F(VectorTest, EraseSingleElement) {
    Vector<int> v{0, 1, 2, 3, 4, 5};
    
    // Erase from middle
    auto it = v.erase(v.begin() + 2);
    EXPECT_EQ(*it, 3);  // Iterator points to element that moved into erased position
    EXPECT_EQ(v.size(), 5);
    
    // Should be: 0, 1, 3, 4, 5
    std::vector<int> expected{0, 1, 3, 4, 5};
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], expected[i]);
    }
    
    // Erase from beginning
    it = v.erase(v.begin());
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    
    // Erase last element
    v.erase(v.end() - 1);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.back(), 4);
}

TEST_F(VectorTest, EraseRange) {
    Vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // Erase range in middle
    auto it = v.erase(v.begin() + 2, v.begin() + 5);
    EXPECT_EQ(*it, 5);  // Element that moved into position
    EXPECT_EQ(v.size(), 7);
    
    // Should be: 0, 1, 5, 6, 7, 8, 9
    std::vector<int> expected{0, 1, 5, 6, 7, 8, 9};
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], expected[i]);
    }
    
    // Test erase empty range
    size_t sizeBefore = v.size();
    it = v.erase(v.begin() + 2, v.begin() + 2);
    EXPECT_EQ(v.size(), sizeBefore);
    EXPECT_EQ(it, v.begin() + 2);
}

// Iterator Tests
TEST_F(VectorTest, Iterators) {
    Vector<int> v{1, 2, 3, 4, 5};
    
    // Forward iteration
    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    // Const iteration
    const Vector<int>& cv = v;
    expected = 1;
    for (auto it = cv.begin(); it != cv.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    // Const iterators
    expected = 1;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    // Reverse iteration
    expected = 5;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
    
    // Const reverse iteration
    expected = 5;
    for (auto it = cv.rbegin(); it != cv.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
    
    // Const reverse iterators
    expected = 5;
    for (auto it = v.crbegin(); it != v.crend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
    
    // Range-based for loop
    expected = 1;
    for (const auto& elem : v) {
        EXPECT_EQ(elem, expected++);
    }
}

// Swap Test
TEST_F(VectorTest, Swap) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{4, 5, 6, 7, 8};
    
    // Store original data
    size_t v1_size = v1.size();
    size_t v2_size = v2.size();
    size_t v1_capacity = v1.capacity();
    size_t v2_capacity = v2.capacity();
    
    v1.swap(v2);
    
    // Sizes should be swapped
    EXPECT_EQ(v1.size(), v2_size);
    EXPECT_EQ(v2.size(), v1_size);
    EXPECT_EQ(v1.capacity(), v2_capacity);
    EXPECT_EQ(v2.capacity(), v1_capacity);
    
    // Contents should be swapped
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(v1[i], i + 4);
    }
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(v2[i], i + 1);
    }
    
    // Test free function swap
    swap(v1, v2);
    
    // Should be back to original state
    EXPECT_EQ(v1.size(), v1_size);
    EXPECT_EQ(v2.size(), v2_size);
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(v1[i], i + 1);
    }
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(v2[i], i + 4);
    }
}

// Comparison Operators Test
TEST_F(VectorTest, ComparisonOperators) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 2, 3};
    Vector<int> v3{1, 2, 4};
    Vector<int> v4{1, 2};
    
    // Equality
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 == v4);
    
    // Inequality
    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);
    EXPECT_TRUE(v1 != v4);
    
    // Empty vectors
    Vector<int> empty1, empty2;
    EXPECT_TRUE(empty1 == empty2);
    EXPECT_FALSE(empty1 != empty2);
    EXPECT_FALSE(empty1 == v1);
}

// Algorithm Compatibility Test
TEST_F(VectorTest, AlgorithmCompatibility) {
    Vector<int> v{5, 2, 8, 1, 9, 3, 7, 4, 6};
    
    // Test std::sort
    std::sort(v.begin(), v.end());
    
    for (int i = 0; i < 9; ++i) {
        EXPECT_EQ(v[i], i + 1);
    }
    
    // Test std::find
    auto it = std::find(v.begin(), v.end(), 5);
    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 5);
    
    // Test std::reverse
    std::reverse(v.begin(), v.end());
    for (int i = 0; i < 9; ++i) {
        EXPECT_EQ(v[i], 9 - i);
    }
    
    // Test std::count
    Vector<int> v2{1, 2, 2, 3, 2, 4};
    int count = std::count(v2.begin(), v2.end(), 2);
    EXPECT_EQ(count, 3);
}

// Edge Cases and Error Handling Tests

TEST_F(VectorTest, EdgeCases) {
    // Empty vector operations
    Vector<int> empty;
    EXPECT_NO_THROW(empty.clear());
    EXPECT_NO_THROW(empty.pop_back());
    EXPECT_NO_THROW(empty.shrink_to_fit());

    // Large vector operations
    Vector<int> large;
    for (int i = 0; i < 1000; ++i) {
        large.push_back(i);
    }
    EXPECT_EQ(large.size(), 1000);
    EXPECT_EQ(large[999], 999);

    // Capacity growth
    Vector<int> growth;
    size_t prev_capacity = growth.capacity();
    for (int i = 0; i < 100; ++i) {
        growth.push_back(i);
        if (growth.capacity() < prev_capacity) {
            ADD_FAILURE() << "Capacity should not decrease.";
        }
        prev_capacity = growth.capacity();
    }
    EXPECT_EQ(growth.size(), 100);
}


// Performance-related tests
TEST_F(VectorTest, ReserveEfficiency) {
    Vector<int> v1, v2;
    
    // Without reserve
    for (int i = 0; i < 1000; ++i) {
        v1.push_back(i);
    }
    
    // With reserve
    v2.reserve(1000);
    for (int i = 0; i < 1000; ++i) {
        v2.push_back(i);
    }
    
    // Both should have same content
    EXPECT_EQ(v1.size(), v2.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

// String-specific tests
TEST_F(VectorTest, StringVector) {
    Vector<std::string> v;
    
    v.push_back("Hello");
    v.emplace_back("World");
    v.insert(v.begin() + 1, "Beautiful");
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], "Hello");
    EXPECT_EQ(v[1], "Beautiful");
    EXPECT_EQ(v[2], "World");
    
    // Test string operations
    v[0] += " there";
    EXPECT_EQ(v[0], "Hello there");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}