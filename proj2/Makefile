# Compiler&flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include

# Linker flags (add -lexpat to the linker flags)
LDFLAGS = -lexpat

# Directories for object files and binaries
OBJDIR = obj
BINDIR = bin
SRCDIR = src
TESTDIR = testsrc
INCLUDEDIR = include

# Source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)

TESTS = $(wildcard $(TESTDIR)/*Test.cpp)
# Object files (automatically generated from source files)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o) $(TESTS:$(TESTDIR)/%.cpp=$(OBJDIR)/%.o)

# Test executables
# TARGETS = $(BINDIR)/teststrutils $(BINDIR)/teststrdatasource $(BINDIR)/teststrdatasink $(BINDIR)/testdsv $(BINDIR)/testxml
TARGETS = $(BINDIR)/testdsv $(BINDIR)/testxml

all: $(TARGETS)

# create directories if nonexistent
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
$(OBJDIR)/%.o: $(TESTDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link executables
# $(BINDIR)/teststrutils: $(OBJDIR)/StringUtils.o $(OBJDIR)/StringUtilsTest.o | $(BINDIR)
# 	$(CXX) $^ -lgtest -lgtest_main -o $@

# $(BINDIR)/teststrdatasource: $(OBJDIR)/StringDataSource.o $(OBJDIR)/StringDataSourceTest.o | $(BINDIR)
# 	$(CXX) $^ -lgtest -lgtest_main -o $@

# $(BINDIR)/teststrdatasink: $(OBJDIR)/StringDataSink.o $(OBJDIR)/StringDataSinkTest.o | $(BINDIR)
# 	$(CXX) $^ -lgtest -lgtest_main -o $@

$(BINDIR)/testdsv: $(OBJDIR)/DSVReader.o $(OBJDIR)/DSVWriter.o $(OBJDIR)/DSVTest.o $(OBJDIR)/StringDataSource.o $(OBJDIR)/StringDataSink.o | $(BINDIR)
	$(CXX) $^ -lgtest -lgtest_main -o $@

$(BINDIR)/testxml: $(OBJDIR)/XMLReader.o $(OBJDIR)/XMLWriter.o $(OBJDIR)/XMLTest.o $(OBJDIR)/StringDataSource.o | $(BINDIR)
	$(CXX) $^ -lgtest -lgtest_main -lexpat -o $@

# run
test: $(TARGETS)
	@for target in $(TARGETS); do \
		./$$target || exit 1; \
	done

# remove obj and bin directories
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	
.PHONY: all test clean