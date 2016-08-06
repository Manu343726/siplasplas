---
layout: default
title: {{ site.name }}
---

# siplasplas

A library for C++ reflection and introspection

## Description

TODO

## Installation

> **NOTE**: siplasplas is a work in progress project
subject to changes. We don't currently provide any kind of
API or ABI guarantee, nor a production-ready installation process.
The following instructions are to build siplasplas from sources.

### Prerequisites

 - **Python 2.7**: The siplasplas relfection engine uses a libclang-based
   parser witten in python. Python 2.7 and pip for Python 2.7 are neccesary.
   All dependencies are handled automatically (See configure instructions bellow).

 - **Mercurial**: The [Entropia Filesystem Watcher](https://bitbucket.org/SpartanJ/efsw) dependency
   is hosted on bitbucket using Mercurial for source control. Mercurial is needed to download the
   dependency.

 - **Doxygen**: Needed only if documentation build is enabled. See *configuration* bellow.

### Dependencies

All siplasplas dependencies are managed automatically through CMake, user doesn't have to care about
dependencies installation. Anyway, here is the list of the thrid party dependencies of siplasplas:

 - [backward-cpp](https://github.com/bombela/backward-cpp)
 - [chaiscript](http://chaiscript.com/) (For examples only)
 - [cmake](https://github.com/Manu343726/cmake)
 - [ctti](https://github.com/Manu343726/ctti)
 - [efsw](https://bitbucket.org/SpartanJ/efsw)
 - [fmt](http://fmtlib.net/latest/index.html)
 - [googletest](https://github.com/google/googletest) (For tests only)
 - [imgui](https://github.com/ocornut/imgui) (For examples only)
 - [SFML](http://www.sfml-dev.org/) (For examples only)
 - [JSON For Modern C++](https://github.com/nlohmann/json)
 - [libexecstream](http://libexecstream.sourceforge.net/)
 - [readerwriterqueue](https://github.com/cameron314/readerwriterqueue)
 - [spdlog](https://github.com/gabime/spdlog)
 - [standardese](https://github.com/foonathan/standardese) (For documentation only)
