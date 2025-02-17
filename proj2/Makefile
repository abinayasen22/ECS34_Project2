# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Executable files
TESTS = teststrutils teststrdatasource teststrdatasink testdsv testxml

# Default target
all: $(TESTS)

# Pattern rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pattern rule for test executables
$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	$(CXX) $(CXXFLAGS) $< -o $@

# Test targets
teststrutils: $(BIN_DIR)/teststrutils
	$(BIN_DIR)/teststrutils

teststrdatasource: $(BIN_DIR)/teststrdatasource
	$(BIN_DIR)/teststrdatasource

teststrdatasink: $(BIN_DIR)/teststrdatasink
	$(BIN_DIR)/teststrdatasink

testdsv: $(BIN_DIR)/testdsv
	$(BIN_DIR)/testdsv

testxml: $(BIN_DIR)/testxml
	$(BIN_DIR)/testxml

# Clean target
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*

# Phony targets
.PHONY: all clean $(TESTS)