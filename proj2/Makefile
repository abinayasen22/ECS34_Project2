# Makefile for ECS34 Project 2

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
LDFLAGS =

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DOC_DIR = docs

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Executables
TESTS = teststrutils teststrdatasource teststrdatasink testdsv testxml

# Default target
all: $(TESTS)

# Include dependency files
-include $(DEPS)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Rules to build test executables
teststrutils: $(OBJ_DIR)/teststrutils.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $(BIN_DIR)/$@

teststrdatasource: $(OBJ_DIR)/teststrdatasource.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $(BIN_DIR)/$@

teststrdatasink: $(OBJ_DIR)/teststrdatasink.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $(BIN_DIR)/$@

testdsv: $(OBJ_DIR)/testdsv.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $(BIN_DIR)/$@

testxml: $(OBJ_DIR)/testxml.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ -o $(BIN_DIR)/$@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(DOC_DIR)

# Create documentation
docs:
	@mkdir -p $(DOC_DIR)
	doxygen Doxyfile

.PHONY: all clean docs