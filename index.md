---
layout: default
title: {{ site.name }}
---

# siplasplas [![Build Status](https://travis-ci.org/Manu343726/siplasplas.svg?branch=master)](https://travis-ci.org/Manu343726/siplasplas) [![Build status](https://ci.appveyor.com/api/projects/status/d395bonrvrduwl6a?svg=true)](https://ci.appveyor.com/project/AlvarBer/siplasplas)

A library for C++ reflection and introspection

## Features

TODO: Examples, more detailed list, etc

 - static reflection of C++ classes, enums, and functions
 - dynamic reflection of C++ namespaces, classes, enums, and functions
 - Multithread message passing
 - Signals and slots
 - cmake access API
 - Runtime C++ compilation (wip)

## Supported compilers

siplasplas has been tested in GCC 5.1/5.2/6.1, Clang 3.6/3.7/3.8, and Visual Studio 2015.

## Documentation

Documentation is available [here]({{site.url}}{{site.baseurl}}/doc/).

The documentation is available in Doxygen and Standardese format, each one
with multiple versions corresponding to the latest documentation of each
siplasplas branch.

> There are no siplasplas releases yet, so there's no stable
> documentation. However, you can consider the `master` branch the most
> stable one.

## Installation

> **NOTE**: siplasplas is a work in progress project
subject to changes. We don't currently provide any kind of
API or ABI stability guarantee, nor a production-ready installation
process. The following instructions are to build siplasplas from sources.

### TL;DR

``` bash
$ git clone https://github.com/Manu343726/siplasplas --recursive
$ cd siplasplas
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

### Prerequisites

 - **Python 2.7**: The siplasplas relfection engine uses a libclang-based
   parser witten in python. Python 2.7 and pip for Python 2.7 are
   neccesary. All dependencies are handled automatically
   (See*configuration* bellow).

 - **Mercurial**: The [Entropia Filesystem Watcher](https://bitbucket.org/SpartanJ/efsw) dependency
   is hosted on bitbucket using Mercurial for source control. Mercurial is needed to download the
   dependency.

 - **Doxygen**: Needed only if documentation build is enabled. See *configuration* bellow.

 - **Libclang**: Siplasplas will use the libclang library distributed as
   part of the system clang installation by default, but it can be
   configured to download and build libclang automatically. See
   *configuration*.

### Dependencies

All siplasplas dependencies are managed automatically through CMake, users
don't have to worry about installing deps. Anyway, here is the list of the
thrid party dependencies of siplasplas:

 - [backward-cpp](https://github.com/bombela/backward-cpp) for exception
   stack traces
 - [chaiscript](http://chaiscript.com/) (For examples only)
 - [cmake](https://github.com/Manu343726/cmake) tools for the cmake
   scripts
 - [ctti](https://github.com/Manu343726/ctti) for type indexing and
   debugging
 - [efsw](https://bitbucket.org/SpartanJ/efsw) for runtime C++ compilation
 - [fmt](http://fmtlib.net/latest/index.html) for diagnostic messages
 - [googletest](https://github.com/google/googletest) (For tests only)
 - [imgui](https://github.com/ocornut/imgui) (For examples only)
 - [SFML](http://www.sfml-dev.org/) (For examples only)
 - [JSON For Modern C++](https://github.com/nlohmann/json) for cmake
   target properties and serialization
 - [libexecstream](http://libexecstream.sourceforge.net/) for cmake
   invocation
 - [readerwriterqueue](https://github.com/cameron314/readerwriterqueue)
   for inter-thread message passing
 - [spdlog](https://github.com/gabime/spdlog) for logging
 - [standardese](https://github.com/foonathan/standardese) (For documentation only)

siplasplas also depends on some python modules:

 - [clang](https://pypi.python.org/pypi/clang) for C++ parsing
 - [colorama](https://pypi.python.org/pypi/colorama) for parser logging
 - [asciitree](https://pypi.python.org/pypi/asciitree/0.3.2) for AST
   debugging
 - [jinja2](http://jinja.pocoo.org/) for code generation

### Download and configure the project

Clone the [siplasplas repository]({{site.project.url}})

``` bash
$ git clone https://github.com/Manu343726/siplasplas --recursive
```

Create a `build/` directory inside the local repository

``` bash
$ cd siplasplas
$ mkdir build
```

Run cmake in the build directory

``` bash
$ cd build
$ cmake ..
```

> Make sure you installed all the requirements before running cmake,
> siplasplas configuration may fail if one or more of that requirements is
> missing.

To build the library, invoke the default build target:

``` bash
$ cmake --build . # Or just "make" if using Makefiles generator
```

### Configuration

The default cmake invocation will build siplasplas as dynamic libraries
(one per module) using the default generator. Also, siplasplas
configuration can be modified using some options and variables:

> The syntax to pass variables to cmake during configuration is
> `-D<VARIABLE>=<VALUE>`, for example:
>
> `$ cmake .. -DSIPLASPLAS_VERBOSE_CONFIG=ON`

 - `CMAKE_BUILD_TYPE`: Build type to be used to build the project (Debug,
   Release, etc). Set to `Debug` by default.
 - `SIPLASPLAS_VERBOSE_CONFIG`: Configure siplasplas using detailed
   output. `OFF` by default.
 - `SIPLASPLAS_LIBRARIES_STATIC`: Build static libraries. `FALSE` by
   default.
 - `SIPLASPLAS_BUILD_EXAMPLES`: Build siplasplas examples in addition to
   libraries. `OFF` by default.
 - `SIPLASPLAS_BUILD_TESTS`: Build siplasplas unit tests. `OFF` by default.
 - `SIPLASPLAS_BUILD_DOCS`: Generate targets to build siplasplas
   documentation. `OFF` by default.
 - `SIPLASPLAS_INSTALL_DRLPARSER_DEPENDENCIES`: Install reflection parser
   python dependencies. `ON` by default. This needs pip version 2.7
   installled.  Dependencies can be manually installed too, there's is
   a `requirements.txt` file in `<siplasplas
   sources>/src/reflection/parser/`. The requirements file doesn't cover
   the `clang` dependency, you must install the clang package **with the
   same version of your installed libclang**. For example, given:

   ``` bash
   $ clang --version
   clang version 3.8.0 (tags/RELEASE_380/final)
   ...
   ```

   you must install `clang==3.8.0` package for Python 2.7.

 - `SIPLASPLAS_DOWNLOAD_LIBCLANG`: Download libclang from LLVM repository.
   If enabled, siplasplas will download LLVM+Clang version
   `${SIPLASPLAS_LIBCLANG_VERSION}` from the LLVM repositories. This
   overrides `SIPLASPLAS_LIBCLANG_INCLUDE_DIR`,
   `SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR`, and
   `SIPLASPLAS_LIBCLANG_LIBRARY` variables. `OFF` by default.

 - `SIPLASPLAS_LIBCLANG_VERSION`: Version of libclang used by the
   reflection parser. Inferred from the installed clang version by
   default.  
   > **NOTE:** siplasplas has been tested with libclang 3.7 and 3.8 only.
   > siplasplas sources use C++14 features, a clang version with C++14
   > support is needed. *Actually, the siplasplas configuration uses
   > `-std=c++14` option, which limits the range of supported versions.*

 - `SIPLASPLAS_LIBCLANG_INCLUDE_DIR`: Path to the LLVM includes. When
   building docs, Standardese tool is built using this configuraton too.
   Inferred by default.
 - `SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR`: Path to the installed clang
   includes. When building docs, Standardese tool is built using this
   configuraton too. Inferred by default.
 - `SIPLASPLAS_LIBCLANG_LIBRARY`: Path to the libclang library. When
   building docs, Standardese tool is built using this configuraton too.
   Inferred by default.

## Acknowledgements

Many thanks to:

 - Jonathan "foonathan" Müller, [as always](https://github.com/foonathan/standardese#acknowledgements)
 - George "Concepticide" Makrydakis, for feedback, debates, and "Guys, what the fuck is gitter?"
 - Diego Rodriguez Losada, for feedback, palmeritas, and blue boxes
 - To all my [ByThech
   WM&IP](http://www.by.com.es/watch-mochi/watch-mochi-ip-video-intercomunicacion-digital-con-tecnologia-ip/)
   team mates, for having to suffer me saying "this with reflection would
   be so easy!" every single day, and specifically to Javier Martín and
   Antonio Pérez for feedback
 - All my twitter followers, still there even with docens of tweets a day
   about reflection! Seriously, some of the best people of the C++
   community are there and give me a lot of feedback and ideas
 - Jens Weller and the Fortune God, thanks for accepting my Meeting C++
   2016 talk about siplasplas

## License

siplasplas project is released under the MIT open source license. This
license applies to all C++ sources, CMake scripts, and any other file
except explicitly stated.
