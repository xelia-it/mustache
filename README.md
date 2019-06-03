## Synopsis

The mustache library is a *mustache-inspired* templace engine written in C++11.

This is a _pre-release_ version: until version 1.0 breaking changes are possible.

## Requirements

It requires:
* a GNU compiler with support for C++11
* The nlhomann JSON C++ library: https://github.com/nlohmann/json
* The Catch2 C++ library for testing: https://github.com/catchorg/Catch2

## Installation

Download json.hpp from nlhomann from here:
https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
and put it into project root folder.

Download Catch2 from here:
https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp
and put it into project root folder.

Then type:
```
make
make test
```

Now you can link mustache.so with your C++ source code.

## Implemented standard commands

* ``{{ var }}`` - Variable substitution (escaped)
* ``{{{ var }}}`` - Variable substitution (unescaped)
* ``{{# var }} {{\ var }}`` - Sections (repeated blocks)
* ``{{^ var }} {{\ var }}`` - Inverted Sections
* ``{{> partial }}`` - Partials
* ``{{! comment }}`` - Comments

## Not implemented commands

* Lambdas
* Set Delimiters

## Non-standard commands

``{{< partial }}`` - Templates

Like partials but takes partial to open from a variable.

``{{= var }} {{\ var }}`` - Empty test

Like sections but *do not change context*.
It's used to check if a variable is empty.

``{{0 var }} {{\ var }}`` - Existance tests

It's used to check if a variable exists.
Compared to ``{{= }}`` and ``{{# }}`` it checks the *keys* of the json
objects instead of the *values*. If the key exists it renders the section.

``{{ array[index] }}`` - Array element selection

Prints an element of the array.
I works also for sections. Eg: you can evaluate an array element using:
``{{# array[index] }}``, ``{{^ array[index] }}``, ``{{= array[index] }}``
or ``{{0 array[index] }}``.

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
