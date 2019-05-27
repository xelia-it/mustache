## Synopsis

The mustache library is a *mustache-inspired* templace engine written in C++11.

## Requirements

It requires:
* a GNU compiler with support for C++11
* The nlhomann JSON C++ library: https://github.com/nlohmann/json
* The Catch2 C++ library for testing: https://github.com/catchorg/Catch2

## Installation

Download json.hpp from nlhomann from here:
https://github.com/nlohmann/json/tree/develop/single_include/nlohmannom
and put it into project root folder.

Download Catch2 from here:
https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp
and put it into test/src folder.

Then type:
```
make
make test
```

Now you can link mustache.so with your C++ source code.

## Implemented standard commands

* ``{{ var }}`` - Variable substitution
* ``{{# var }} {{\ var }}`` - Sections (repeated blocks)
* ``{{^ var }} {{\ var }}`` - Inverted Sections
* ``{{> partial }}`` - Partials

## Unimplemented commands

* ``{{! comment }}`` - Comments
* ``{{{ var }}}`` - Unescaped variable substitution
* Lambdas
* Set Delimiters

## Not-standard commands

``{{< partial }}`` - Templates

Like partials but takes partial to open from a variable.

``{{= var }} {{\ var }}`` - Empty test

Like sections but *do not change context*.
It's used to check if a variable is empty.

``{{0 var }} {{\ var }}`` - Existance tests

It's used to check if a variable exists.
Compared to ``{{= }}`` and ``{{# }}`` it checks the *keys* of the json
objects instead of the *values*. If the key exists it renders the section.

## See also

* https://mustache.github.io/ for original language reference.

## TODO

There a number of work planned:
* more tests
* use of git submodule to link to original projects
* implementation of new commands
* improve documentation
* integration with CMake

## License

The library is licenced under the commercial-friendly MIT license.
