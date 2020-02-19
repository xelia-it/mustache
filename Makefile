# Mustache

# Uncomment this line to activate debug
# DEFS += -DDEBUG

prefix = /usr
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib
INSTALL_PROGRAM=/usr/bin/install

# Library variables
LIBRARY_NAME := mustache
LIBRARY_SHARED := lib$(LIBRARY_NAME).so
LIBRARY_STATIC := lib$(LIBRARY_NAME).a
LIBRARY_CPP_FILES := $(wildcard src/*.cpp)
LIBRARY_OBJ_FILES := $(LIBRARY_CPP_FILES:.cpp=.o)

# Interactive mustache
INTERACTIVE_NAME := mustache-interactive

# Test files variables
TEST_NAME := mustache-test
TEST_CPP_FILES := $(wildcard test/src/*.cpp)
TEST_OBJ_FILES := $(TEST_CPP_FILES:.cpp=.o)

# Generic compiling flags
CC_FLAGS := --std=c++11 -fPIC -Wall -Wextra -Wpedantic -Werror $(DEFS)
LD_FLAGS := -l$(LIBRARY_NAME) -L.

# Targets

all: $(LIBRARY_SHARED) $(LIBRARY_STATIC) $(INTERACTIVE_NAME) $(TEST_NAME)

clean:
	rm -f $(LIBRARY_SHARED) $(LIBRARY_STATIC) $(LIBRARY_OBJ_FILES) $(INTERACTIVE_NAME) \
		$(TEST_NAME) $(TEST_OBJ_FILES)

check:
	cppcheck --enable=all --quiet --inconclusive --std=c++11 \
		--suppress=*:*catch.hpp \
	$(TEST_NAME).cpp $(TEST_CPP_FILES) $(LIBRARY_CPP_FILES)

test: all
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):. && ./$(TEST_NAME)

install: $(LIBRARY_SHARED) $(LIBRARY_STATIC)
	[ -d $(DESTDIR)$(libdir) ] || $(INSTALL_PROGRAM) -d $(DESTDIR)$(libdir)/
	$(INSTALL_PROGRAM) $^ $(DESTDIR)$(libdir)/

# Build mustache libraries
$(LIBRARY_SHARED): $(LIBRARY_OBJ_FILES)
	$(CXX) -shared -Wl,-soname,$(LIBRARY_SHARED) -o $@ $^

$(LIBRARY_STATIC): $(LIBRARY_OBJ_FILES)
	ar rc $@ $^
	ranlib $@

# Interactive test program
$(INTERACTIVE_NAME): $(LIBRARY_SHARED) $(INTERACTIVE_NAME).cpp
	$(CXX) $(CC_FLAGS) $(INTERACTIVE_NAME).cpp -o $(INTERACTIVE_NAME) $(LD_FLAGS)

# Build unit test program
$(TEST_NAME): $(LIBRARY_SHARED) $(TEST_OBJ_FILES)
	$(CXX) $(CC_FLAGS) $(TEST_OBJ_FILES) -o $(TEST_NAME) $(LD_FLAGS)

%.o: %.cpp Makefile
	$(CXX) $(CC_FLAGS) -c -o $@ $<
