---
layout: default
title: {{ site.name }}
---

# siplasplas

A library for C++ reflection and introspection

## Description

TODO

## Documentation

Documentation is available [here]({{site.url}}{{site.baseurl}}/doc/).

The documentation is available in Doxygen and Standardese format, each one
with multiple versions corresponding to the latest documentation of each
siplasplas branch.

> There are no siplasplas releases yet, so there's no stable
> documentation. However, you can conider the `master` branch the most
> stable one.

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

## Acknowledgements

Many thanks to:

 - Jonathan "foonathan" Müller, [as always](https://github.com/foonathan/standardese#acknowledgements)
 - George "Concepticide" Makridakis, for feedback, debates, and "Guys, what the fuck is gitter?"
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
