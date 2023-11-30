Mustache Light
==============

## Synopsis

The Mustache library is a *mustache-inspired* templace engine written in C++.

This is a _pre-release_ version: until version 1.0 breaking changes are possible.


## Requirements

It requires:
* a GNU compiler with support for C++11

It depends from:
* The nlhomann JSON C++ library: https://github.com/nlohmann/json
* The Catch2 C++ library for testing: https://github.com/catchorg/Catch2


## Quick start

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


## Documentation

Please check (docs folder)[./docs] for detailed documentation.


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
