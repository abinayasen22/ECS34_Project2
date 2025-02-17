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

# create directories if nonexistent
$(OBJDIR):
	mkdir -p $(OBJDIR)

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
