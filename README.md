# siplasplas [![Build Status](https://travis-ci.org/Manu343726/siplasplas.svg?branch=master)](https://travis-ci.org/Manu343726/siplasplas) [![Build status](https://ci.appveyor.com/api/projects/status/d395bonrvrduwl6a?svg=true)](https://ci.appveyor.com/project/AlvarBer/siplasplas) [![docs](https://codedocs.xyz/Manu343726/siplasplas.svg)](https://codedocs.xyz/Manu343726/siplasplas/)

Examples and utilities for the Advanced C++ course for the GUEIM association, Complutense University of Madrid.

## Contents

The repository is organized as a small library with the different data structures, examples, and other utilities that will be studied in the course. In addition to hosting the course's contents, it is modeled to demonstrate how a typical Open Source C++ proyect is organized (Build system, testing, CI, etc).

The repo is structured as follows:

 - **`include/`**: Headers of the different utilities and examples.
 - **`src/`**: Implementations.
 - **`examples/`**: Examples of usage.
 - **`tests/`**: Unit tests.
 - **`3rdParty/`**: External dependencies.

## Environments

Since the course covers C++14, it is mandatory that the used compiler has at least partial support of C++14. I recommend using the newest compiler available to you. The contents have been tested for the following compilers:

### GCC 5.2-5.3
Should already be available in any Linux distro. In the case of Ubuntu, you may add the testing PPAs (`ubuntu-toolchain-r-test`) to install it.
You can see the installation process for Debian in the Travis CI build.

It is available for Windows trough the [nuwen MinGW distro](http://nuwen.net/mingw.html).

In case it is not available (in Cygwin, for example), you may try to compile GCC by hand. It's really simple:

``` bash
$ git clone https://github.com/gcc-mirror/gcc gcc && cd gcc
$ git checkout tag/gcc_5_3_0_release
$ cd ..
$ mkdir gcc-buildtree && cd gcc-buildtree
$ ../gcc/configure --program-suffix=-5.3 --enable_languages=c++ --enable-version-specific-runtime-libs
$ make -j8
$ make check
$ make install
```

[Here](http://eli.thegreenplace.net/2014/01/16/building-gcc-4-8-from-source-on-ubunu-12-04/) is a good guide for Ubuntu.

### Clang 3.6 forward

Just like GCC, you may download it or compile it by hand. As an example, the Travis build installs Clang 3.7 in Ubuntu using the testing PPA. In the comments, you can find the instructions to compile LLVM and Clang by hand.

### Visual Studio 2015

Microsoft has (almost) completed the implementation of C++11 since Visual Studio 2015 Update 1.
C++14 is partially supported. For the course, you need [at least VS2015](https://ci.appveyor.com/project/Manu343726/siplasplas/build/1.0.9).

## Setup

[![asciicast](https://asciinema.org/a/c13nlez3fhd86xdkicw7l6y8q.png)](https://asciinema.org/a/c13nlez3fhd86xdkicw7l6y8q)

Just clone the repo and initialize the submodules of the external dependencies:

``` bash
$ git clone https://github.com/GueimUCM/siplasplas
$ cd siplasplas
$ git submodule update --init
```

or, just in one step:

``` bash
$ git clone --recursive https://github.com/GueimUCM/siplasplas
```

To compile the examples and the unit tests, use the typical out-of-source compilation method of cmake:

``` bash
user@siplasplas: $ mkdir build && cd build
user@siplasplas/build: $ cmake ..
user@siplasplas/build: $ cmake --build .
```
*If you are working with more than one environment at a time, it is recommended that you have a build folder for each environment. For example, if you're working under Windows you might want to try everything with Visual Studio, MinGW (GCC), and Cygwin (GCC and Clang), with a build folder for each one:*

``` bash
user@siplasplas: $ mkdir buildVS && cd buildVS
user@siplasplas/buildVS: $ cmake ..
user@siplasplas: $ mkdir buildMinGW && cd buildMinGW
user@siplasplas/buildMinGW: $ cmake ..
cygwin@siplasplas: $ mkdir buildCygwin && cd buildCygwin
cygwin@siplasplas/buildCygwin: $ cmake ..
```
*You may also configure a project to act as an "umbrella", such that it configures all the projects you need automatically. An example is available [here](https://github.com/Manu343726/cpp-dod-tests).*

Even though you can run the unit tests directly by running the executables, they are added to the project's [ctest](https://cmake.org/Wiki/CMake/Testing_With_CTest) configuration:

``` bash
user@siplasplas/build: $ ctest . -VV
```

*The generator of Visual Studio is multi-configuration, so you will have to tell CTest which configuration (Release or Debug) is it that you want to test:*

``` bash
user@siplasplas/build: $ ctest -C Debug . -VV
```

*You must take into accout that GTest and GMock are built for a specific configuration at compile time (that is, the `${CMAKE_BUILD_TYPE}` variable), so make sure that you have configured the project in the corresponding mode before launching the test, since it won't link otherwise.*

### Reflection

The reflection engine demonstrated in the course uses a Python script to analyze and generate the exta code necessary for refletion. In addition, said script uses the libclang API to analyze source code.
For that reason, in order to compile and run the reflection examples, you must have Python 2.7 and libclang installed, as well as the script dependencies:

1. **Install Python:**

In Linux, it should be enough to install it via your package manager. In Windows, you can download it from [here](https://www.python.org/downloads/). It's really important that you make sura that both the interpreter and the scripts are added to the `PATH`. Under Windows, the default configuration is `C:\Python27` and `C:\Python27\scripts`.

Besides, it is necessary to install `pip`, the Python dependency manager. The typical method is to download and execute the [`get-pip.py`](https://bootstrap.pypa.io/get-pip.py) script:

``` shell
$ python get-pip.py
```

2. **Install libclang:**

 - Windows: The precompiled version of Clang includes libclang, it should work out of the box. If it doesn't the problem probably lies in the path of `libclang.so`. Both the parser script and the CMake interface include a flag to pass the route of `libclang.so`, in case that happens. You may download Clang for Windows [here](http://llvm.org/releases/download.html).
 - Linux: The name of the package may change from one distro to another. In Debian, for example, the one being used in the Travis builds is `libclang1-[VERSION CLANG]`. It is usually included as part of the clang package.

3. **Install dependencies:**

The script includes a file `requirements.txt` with it's dependencies: 

``` shell
$ pip install -r include/reflection/parser/requirements.txt
```

### 3rdParty/cmake

Is a small repo where I have been uploading utilities to help using CMake. At the moment, it contains:

 - `exec-targets.cmake`: Defines a function `exec_target(NAME <name> PREFIX <prefix> [COMPILE_OPTIONS <compiler options>])` which creates an executable target `<prefix>_<name>` from the file `<prefix>s/<name>.cpp`. For example:
 
 ``` cmake
 exec_target(NAME potato PREFIX example)
 ```
  
  creates an executable `example_potato` from the file `examples/potato.cpp`. 
  To be able to use that function, you must call `setup_exec_targets([PROJECT <project name>] ...)` with the configuration that you want to use (Source folder, include directory, tests directory, etc). In any case, it is designed for projects with the organization mentioned above, so a call without parameters should work.

 - `gmock.cmake`: Downloads and configures Google's mocking and unit testing framework ([Google Test Framework](https://github.com/google/googletest)), so that it's really simple to write our own tests. It contains two functions `install_gtestgmock([VERSION <version>] ...)` (Downloads and configures the framework for our project) and `gmock_test_target(NAME <name>)` (A wrapper for `exec_target()` that configures a test executable  and links it to GTest/GMock).
 - `vs_source_groups.cmake`: The Visual Studio generator doesn't take the folder structure of a target's sources/includes into account when generating the corresponding project. This is really bothersome, since it means that usually only the `.cpp`s appear, without maintaining the folder structure. The function `generate_vs_source_groups(<group name> <top directory>)` configures the project to follow the original folder structure. It is called automatically from `exec_target()` if you are using Visual Studio.
 - [`cmakepp.cmake`](https://github.com/toeb/cmakepp): A framework that extends CMake with lots of utilities. Although it's complex and lacks documentation, the creator usually answers questions in detail. *I have copied the compiled version of cmakepp so that I can modify some things, that's why it's not a submodule.*
 - `custom_properties.cmake`: Both the targets and the sources of a CMake project have a series of properties associated to them, that we can query, modify, etc. However, the property set is fixed, we cannot add our own. This module implements a series of functions similar to the CMake property interface to defin our own properties for files and targets.

## License

All contents shared here are published under the MIT License

