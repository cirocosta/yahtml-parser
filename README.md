# yahtml-parser

> A naive C++ HTML parser implementation using Bison and Flex

**ATTENTION**: this is NOT a full spec conformant parser. It DOES NOT aim to comply with all of the requirements associated with the roles it partakes in the HTML specification.

[![Build Status](https://travis-ci.org/cirocosta/yahtml-parser.svg?branch=master)](https://travis-ci.org/cirocosta/yahtml-parser)

## Install

Make sure that you have all the dependencies installed:

- flex
- bison\*
- make
- cmake
- a modern c++ compiler (gcc4.9 / gcc5.2)

Then, simply:

```sh
$ mkdir build
$ cd build
$ cmake ..   (cmake -Dtest=ON .. in case you want tests)
$ make && make test
```

\*: If you've built `bison` from source and in the `make` step it claims that there's a problem with `m4`, try setting the `pkgdata_dir`:
```
export BISON_PKGDATADIR=/usr/local/share/bison/data
```

## CMake

If you wish to include `yahtml-parser` in your project:

```
yahtml-parser_INCLUDES  : headers to be included
yahtml-parser_LIBS      : libs to be linked against
```

## LICENSE

GPLv2


