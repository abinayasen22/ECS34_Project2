# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -I./include -g
LDFLAGS = -lexpat

# Directories
SRC_DIR = src
TEST_DIR = testsrc
OBJ_DIR = obj
BIN_DIR = bin

# Source files and test files
SRC_FILES = $(SRC_DIR)/DSVReader.cpp $(SRC_DIR)/DSVWriter.cpp $(SRC_DIR)/XMLReader.cpp $(SRC_DIR)/XMLWriter.cpp
TEST_FILES = $(TEST_DIR)/DSVTest.cpp $(TEST_DIR)/XMLTest.cpp

# Object files
OBJ_FILES = $(SRC_FILES:.cpp=.o)
TEST_OBJ_FILES = $(TEST_FILES:.cpp=.o)

# Executables
EXEC_TEST_DSV = $(BIN_DIR)/testdsv
EXEC_TEST_XML = $(BIN_DIR)/testxml
EXEC_TEST_STRUTILS = $(BIN_DIR)/teststrutils
EXEC_TEST_STRDATASOURCE = $(BIN_DIR)/teststrdatasource
EXEC_TEST_STRDATASINK = $(BIN_DIR)/teststrdatasink

# Targets
all: $(EXEC_TEST_STRUTILS) $(EXEC_TEST_STRDATASOURCE) $(EXEC_TEST_STRDATASINK) $(EXEC_TEST_DSV) $(EXEC_TEST_XML)

# Rule to link test executables
$(EXEC_TEST_STRUTILS): $(OBJ_FILES) $(TEST_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(TEST_OBJ_FILES) -o $@ $(LDFLAGS)

$(EXEC_TEST_STRDATASOURCE): $(OBJ_FILES) $(TEST_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(TEST_OBJ_FILES) -o $@ $(LDFLAGS)

$(EXEC_TEST_STRDATASINK): $(OBJ_FILES) $(TEST_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(TEST_OBJ_FILES) -o $@ $(LDFLAGS)

$(EXEC_TEST_DSV): $(OBJ_FILES) $(TEST_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(TEST_OBJ_FILES) -o $@ $(LDFLAGS)

$(EXEC_TEST_XML): $(OBJ_FILES) $(TEST_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(TEST_OBJ_FILES) -o $@ $(LDFLAGS)

# Rule to compile object files for source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile object files for test files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object and binary files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run all the tests in order
run_tests: $(EXEC_TEST_STRUTILS) $(EXEC_TEST_STRDATASOURCE) $(EXEC_TEST_STRDATASINK) $(EXEC_TEST_DSV) $(EXEC_TEST_XML)
	$(EXEC_TEST_STRUTILS)
	$(EXEC_TEST_STRDATASOURCE)
	$(EXEC_TEST_STRDATASINK)
	$(EXEC_TEST_DSV)
	$(EXEC_TEST_XML)

.PHONY: clean run_tests all
