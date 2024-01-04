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

# Benchmark files variables
BENCHMARK_NAME := mustache-benchmark
BENCHMARK_CPP_FILES := $(wildcard benchmark/src/*.cpp)
BENCHMARK_OBJ_FILES := $(BENCHMARK_CPP_FILES:.cpp=.o)

# Includes
INCLUDES := \
	-Isrc \
	-Ithird-party/json/single_include/ \
	-Ithird-party/catch2/single_include

# Generic compiling flags
CPP_LANGUAGE_VERSION := c++11
CC_FLAGS := --std=$(CPP_LANGUAGE_VERSION) -fPIC -Wall -Wextra -Wpedantic -Werror $(DEFS) $(INCLUDES)
LD_FLAGS := -l$(LIBRARY_NAME) -L.

# Targets

all: $(LIBRARY_SHARED) $(LIBRARY_STATIC) \
	$(INTERACTIVE_NAME) $(TEST_NAME) $(BENCHMARK_NAME)

clean:
	rm -f $(LIBRARY_SHARED) $(LIBRARY_STATIC) $(LIBRARY_OBJ_FILES) $(INTERACTIVE_NAME) \
		$(TEST_NAME) $(TEST_OBJ_FILES)

distclean: clean

check:
	@if `which cppcheck > /dev/null`; then \
		echo "Checking code with cppcheck"; \
		cppcheck --enable=all --quiet --inconclusive \
			--std=$(CPP_LANGUAGE_VERSION) \
			--suppress=*:third-party/* \
			--suppress=toomanyconfigs \
			--suppress=missingIncludeSystem \
			$(TEST_NAME).cpp $(TEST_CPP_FILES) $(LIBRARY_CPP_FILES); \
	else \
		echo "cppcheck not installed"; \
		false; \
	fi

test: all
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):. && ./$(TEST_NAME)

memcheck: all
	@if `which valgrind > /dev/null`; then \
		echo "Checking code with valgrind"; \
		export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):.; \
		valgrind \
			--leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--error-exitcode=1 ./$(TEST_NAME); \
	else \
		echo "valgrind not installed"; \
		false; \
	fi

benchmark: all
	export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):. && ./$(BENCHMARK_NAME)

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

# Build unit test program
$(BENCHMARK_NAME): $(LIBRARY_SHARED) $(BENCHMARK_OBJ_FILES)
	$(CXX) $(CC_FLAGS) $(BENCHMARK_OBJ_FILES) -o $(BENCHMARK_NAME) $(LD_FLAGS)

%.o: %.cpp Makefile
	$(CXX) $(CC_FLAGS) -c -o $@ $<
