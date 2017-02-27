## Synopsis

The mustache library is a mustache-inspired templace engine written in C++11.

Currently the library is in __beta__ stage.

## Requirements

It requires:
* a GNU compiler with support for C++11
* The nlhomann JSON C++ library: https://github.com/nlohmann/json
* The catch C++ library for testing: https://github.com/philsquared/Catch

## Installation

Copy json.hpp from nlhomann repository into project root folder.
Copy catch.hpp from philsquared repository into test/src folder .
Then type:
```
make
make test
```

Now you can link mustache.so with your C++ source code.

## See also

* https://mustache.github.io/ for original language reference.

## TODO

There a number of work planned:
* more tests
* use of git submodule to link to original projects
* implementation of new commands
* integration with CMake

## License

The library is licenced under the commercial-friendly MIT license.
