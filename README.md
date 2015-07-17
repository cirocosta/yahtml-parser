# yahtmlarser
> A naive C++ HTML parser implementation using Bison and Flex

**ATTENTION**: this is NOT a full spec conformant parser. It DOES NOT aim to comply with all of the requirements associated with the roles it partakes in the HTML specification.

[![Build Status](https://travis-ci.org/cirocosta/yahtml-parser.svg?branch=master)](https://travis-ci.org/cirocosta/yahtml-parser)

## Install

Make sure that you have all the dependencies installed:

- flex
- bison\*
- make
- cmake
- a modern c++ compiler - targetting c++11 (i recommend clang, 3.5 is good)

Then, simply:

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make && make test
```

\*: If you've built `bison` from source and in the `make` step it claims that there's a problem with `m4`, try setting the `pkgdata_dir`:
```
export BISON_PKGDATADIR=/usr/local/share/bison/data
```

## XML-like Documents

When dealing with this kind of documents we have two models for parsing: SAX-like and DOM-like. The first is event-driven, fast and consumes few memory. The later gives us a full tree representation that we can later consume.


## Resources

1. [Stream Processing of XML Documents Made Easy with LALR(1) Parser Generators , Padovani, L., Zac- chiroli, S., Vitali, F., September 2007](https://upsilon.cc/~zack/research/publications/flea.pdf)

## LICENSE

GPLv2


