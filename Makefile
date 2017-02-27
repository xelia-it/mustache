# Mustache

# Uncomment this line to activate debug
# DEFS += -DDEBUG

# Library variables
LIBRARY_SONAME := mustache
LIBRARY_NAME := lib$(LIBRARY_SONAME).so
LIBRARY_CPP_FILES := $(wildcard src/*.cpp)
LIBRARY_OBJ_FILES := $(LIBRARY_CPP_FILES:.cpp=.o)

# Interactive mustache
INTERACTIVE_NAME := mustache-interactive

# Test files variables
TEST_NAME := mustache-test
TEST_CPP_FILES := $(wildcard test/src/*.cpp)
TEST_OBJ_FILES := $(TEST_CPP_FILES:.cpp=.o)

# Generic compiling flags
CC_FLAGS := -I. -I./src --std=c++11 -fPIC $(DEFS)
LD_FLAGS := -l$(LIBRARY_SONAME) -L.

# Targets

all: $(LIBRARY_NAME) $(INTERACTIVE_NAME) $(TEST_NAME)

clean:
	rm -f $(LIBRARY_NAME) $(LIBRARY_OBJ_FILES) $(INTERACTIVE_NAME) \
		$(TEST_NAME) $(TEST_OBJ_FILES)

check:
	cppcheck --enable=all --quiet --inconclusive --std=c++11 \
		--suppress=*:*catch.hpp \
	$(TEST_NAME).cpp $(TEST_CPP_FILES) $(LIBRARY_CPP_FILES)

test: all
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):. && ./$(TEST_NAME)

# Build mustache library
$(LIBRARY_NAME): $(LIBRARY_OBJ_FILES)
	$(CXX) -shared -Wl,-soname,$(LIBRARY_NAME) -o $(LIBRARY_NAME) -o $@ $^

# Interactive test program
$(INTERACTIVE_NAME): $(LIBRARY_NAME) $(INTERACTIVE_NAME).cpp
	$(CXX) $(CC_FLAGS) $(INTERACTIVE_NAME).cpp -o $(INTERACTIVE_NAME) $(LD_FLAGS)

# Build unit test program
$(TEST_NAME): $(LIBRARY_NAME) $(TEST_OBJ_FILES)
	$(CXX) $(LD_FLAGS) $(TEST_OBJ_FILES) -o $(TEST_NAME)

%.o: %.cpp
	$(CXX) $(CC_FLAGS) -c -o $@ $<
