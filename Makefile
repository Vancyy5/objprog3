# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

# Google Test
GTEST_DIR = external/googletest/googletest
GTEST_INCLUDE = $(GTEST_DIR)/include
GTEST_SRC = $(GTEST_DIR)/src/gtest-all.cc

# Project source files
DEPS = zmogus.h funkcijos.h laikas.h vector.h
SRC = main.cpp funkcijos.cpp zmogus.cpp laikas.cpp vector.cpp
OBJ = $(SRC:.cpp=.o)

# Test files
TEST_SRC1 = testai.cpp
TEST_SRC2 = vector_testai.cpp
TEST_OBJ1 = $(TEST_SRC1:.cpp=.o) funkcijos.o zmogus.o laikas.o gtest-all.o
TEST_OBJ2 = $(TEST_SRC2:.cpp=.o) vector.o laikas.o gtest-all.o

# Targets
TARGET = main
TEST_TARGET1 = run_tests
TEST_TARGET2 = vector_tests

# Temporary files
TEMP_FILES = failinis.txt studentai.txt rezultataiT.txt

# Default target
all: $(TARGET)

# Main executable
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Compile source files
%.o: %.cpp $(DEPS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Compile gtest
gtest-all.o: $(GTEST_SRC)
	$(CXX) -I$(GTEST_INCLUDE) -I$(GTEST_DIR) -c $< -o $@

# Compile test sources
testai.o: $(TEST_SRC1) $(DEPS)
	$(CXX) -I$(GTEST_INCLUDE) -I$(GTEST_DIR) -c $< -o $@

vector_testai.o: $(TEST_SRC2) $(DEPS)
	$(CXX) -I$(GTEST_INCLUDE) -I$(GTEST_DIR) -c $< -o $@

# Link test executables
$(TEST_TARGET1): $(TEST_OBJ1)
	$(CXX) -o $@ $^ -pthread

$(TEST_TARGET2): $(TEST_OBJ2)
	$(CXX) -o $@ $^ -pthread

# Test targets
test: $(TEST_TARGET1)
	./$(TEST_TARGET1)

vector_test: $(TEST_TARGET2)
	./$(TEST_TARGET2)

# Clean up
clean:
	rm -f $(OBJ) $(TEST_OBJ1) $(TEST_OBJ2) $(TARGET) $(TEST_TARGET1) $(TEST_TARGET2) $(TEMP_FILES)

# Run main
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run test vector_test
