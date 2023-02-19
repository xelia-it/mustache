Mustache Light
==============

## Synopsis

The mustache library is a *mustache-inspired* templace engine written in C++11.

This is a _pre-release_ version: until version 1.0 breaking changes are possible.

## Requirements

It requires:
* a GNU compiler with support for C++11

It depends from:
* The nlhomann JSON C++ library: https://github.com/nlohmann/json
* The Catch2 C++ library for testing: https://github.com/catchorg/Catch2

## Installation

Clone the repo with:
```
git clone https://github.com/xelia-it/mustache.git --recursive --shallow-submodules --depth=1
```

Then type:
```
make
```

If you want to run unit tests run:
```
make test
```

Now you can link mustache.so with your C++ source code.

## LSP Integration

For LSP (Language Server protocol) integration it's useful to have  add a _compile_flags.txt_ file in the root diretory of the project.
The suggested content is:

```
-std=c++11
-fPIC
-Ithird-party/json/single_include
-Ithird-party/catch2/single_include
```

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
- [ ] more tests
- [x] use of git submodule to link to original projects
- [ ] implementation of new commands
- [ ] finalize v1.0 interface
- [ ] improve documentation
- [ ] integration with CMake

## License

The library is licenced under the commercial-friendly MIT license.
