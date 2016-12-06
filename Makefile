# Mustache

# Uncomment this line to activate debug
# DEFS += -DDEBUG

# Other definition
LIBRARY_SONAME := mustache
LIBRARY_NAME := lib$(LIBRARY_SONAME).so
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)

INTERACTIVE_NAME := mustache-interactive

TEST_NAME := mustache-test
TEST_CPP_FILES := $(wildcard test/src/*.cpp)
TEST_OBJ_FILES := $(TEST_CPP_FILES:.cpp=.o)

CC_FLAGS := -I. -I./src --std=c++11 -fPIC $(DEFS)
LD_FLAGS := -l$(LIBRARY_SONAME) -L.

# Targets

all: $(LIBRARY_NAME) $(INTERACTIVE_NAME) $(TEST_NAME)

clean:
	rm -f $(OBJ_FILES) $(TEST_OBJ_FILES) $(TEST_NAME) $(INTERACTIVE_NAME) $(LIBRARY_NAME)

check:
	cppcheck --enable=all --quiet --inconclusive --std=c++11 \
	$(TEST_NAME).cpp $(TEST_CPP_FILES) $(CPP_FILES)

# Build mustache library
$(LIBRARY_NAME): $(OBJ_FILES)
	$(CXX) -shared -Wl,-soname,$(LIBRARY_NAME) -o $(LIBRARY_NAME) -o $@ $^

#$(OBJ_FILES): $(CPP_FILES)#
#	$(CXX) $(CC_FLAGS) -c -o $@ $<
%.o: %.cpp %.hpp Makefile
	$(CXX) $(CC_FLAGS) -c -o $@ $<

# Build interactive
$(INTERACTIVE_NAME): $(LIBRARY_NAME) $(INTERACTIVE_NAME).cpp
	$(CXX) $(CC_FLAGS) $(INTERACTIVE_NAME).cpp -o $(INTERACTIVE_NAME) $(LD_FLAGS)

# Build interactive
$(TEST_NAME): $(LIBRARY_NAME) $(TEST_OBJ_FILES)
	$(CXX) $(CC_FLAGS) $(TEST_OBJ_FILES) -o $(TEST_NAME) $(LD_FLAGS)

$(TEST_OBJ_FILES): $(TEST_CPP_FILES)
	$(CXX) $(CC_FLAGS) -c -o $@ $<

