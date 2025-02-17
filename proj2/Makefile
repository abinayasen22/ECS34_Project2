<<<<<<< HEAD
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
=======
# Compiler&flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Directories for object files and binaries
OBJDIR = obj
BINDIR = bin

# Source files
SRCS = StringUtils.cpp StringUtilsTest.cpp \
		StringDataSource.cpp StringDataSourceTest.cpp \
		StringDataSink.cpp StringDataSinkTest.cpp \
		DSVReader.cpp DSVWriter.cpp DSVTest.cpp \
    	XMLReader.cpp XMLWriter.cpp XMLTest.cpp

# Object files (automatically generated from source files)
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

# Test executables
TARGETS = $(BINDIR)/teststrutils $(BINDIR)/teststrdatasource $(BINDIR)/teststrdatasink 
>>>>>>> a6564b33148a13620c25ce2b014c75a5b3715911

# create directories if nonexistent
$(OBJDIR):
	mkdir -p $(OBJDIR)

<<<<<<< HEAD
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
=======
$(BINDIR):
	mkdir -p $(BINDIR)

# build object files
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link executables
$(BINDIR)/teststrutils: $(OBJDIR)/StringUtils.o $(OBJDIR)/StringUtilsTest.o | $(BINDIR)
	$(CXX) $^ -lgtest -lgtest_main -o $@

$(BINDIR)/teststrdatasource: $(OBJDIR)/StringDataSource.o $(OBJDIR)/StringDataSourceTest.o | $(BINDIR)
	$(CXX) $^ -lgtest -lgtest_main -o $@

$(BINDIR)/teststrdatasink: $(OBJDIR)/StringDataSink.o $(OBJDIR)/StringDataSinkTest.o | $(BINDIR)
	$(CXX) $^ -lgtest -lgtest_main -o $@

# run
test: $(TARGETS)
	@for target in $(TARGETS); do \
		./$$target || exit 1; \
	done

# remove obj and bin directories
clean:
	rm -rf $(OBJDIR) $(BINDIR)
>>>>>>> a6564b33148a13620c25ce2b014c75a5b3715911
