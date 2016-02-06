# siplasplas [![Build Status](https://travis-ci.org/GueimUCM/siplasplas.svg?branch=master)](https://travis-ci.org/GueimUCM/siplasplas) [![Build status](https://ci.appveyor.com/api/projects/status/1rf604ixu7drh89t?svg=true)](https://ci.appveyor.com/project/Manu343726/siplasplas)

# English

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

The contents here described are published under the MIT License

# Español

Ejemplos y utilidades del curso de C++ avanzado de la asociación GUEIM, Universidad Complutense de Madrid.

## Contenidos

El repo se organiza como una pequeña biblioteca con las diferentes estructuras de datos, ejemplos, y otras utilidades que veamos a lo largo del curso. La idea es mostrar como se suele organizar un proyecto C++ open source (Build system, testing, CI, etc) a la vez que se hostean los contenidos dados en clase.

La estructura es la siguiente:

 - **`include/`**: Cabezeras de las distintas utilidades, ejemplos, etc. 
 - **`src/`**: Implementación.
 - **`examples/`**: Ejemplos de uso.
 - **`tests/`**: Tests unitarios.
 - **`3rdParty/`**: Dependencias externas.

## Entornos

Dado que el curso cubre C++14, es necesario un compilador con soporte al menos parcial de C++14. Recomiendo utilizar un compilador cuanto más nuevo mejor. Está probada sobre los siguientes compiladores:

### GCC 5.2-5.3
En cualquier distro linux debería estar ya disponible. En el caso de Ubuntu, notable por no actualizar los paquetes, podéis añadir los PPA de testing (`ubuntu-toolchain-r-test`) para instalarlo.  
Podéis ver como se instala en Debian en la build de Travis CI.

En Windows ya está disponible a través de la [distro nuwen de MinGW](http://nuwen.net/mingw.html).

En caso de que no esté disponible (En Cygwin por ejemplo), podéis lanzaros a la piscina y compilar GCC a mano. Es muy sencillo:

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

[Aquí](http://eli.thegreenplace.net/2014/01/16/building-gcc-4-8-from-source-on-ubunu-12-04/) hay una buena guía para Ubuntu.

### Clang 3.6 en adelante

Al igual que GCC, podéis bajarlo o compilarlo a mano. Como ejemplo, la build de travis instala Clang 3.7 en Ubuntu usando la PPA de testing. También están (comentadas) las instrucciones necesarias para compilar LLVM y Clang a mano.

### Visual Studio 2015

Microsoft ha (casi) completado la implementación de C++11 desde Visual Studio 2015 Update 1.
El soporte de C++14 es parcial. Para el curso será necesario [mínimo VS2015](https://ci.appveyor.com/project/Manu343726/siplasplas/build/1.0.9).

## Setup

[![asciicast](https://asciinema.org/a/c13nlez3fhd86xdkicw7l6y8q.png)](https://asciinema.org/a/c13nlez3fhd86xdkicw7l6y8q)

Solo hay que clonar el repo e inicializar los submódulos de las dependencias externas:

``` bash
$ git clone https://github.com/GueimUCM/siplasplas
$ cd siplasplas
$ git submodule update --init
```

o en un solo paso:

``` bash
$ git clone --recursive https://github.com/GueimUCM/siplasplas
```

Para compilar los ejemplos y los test unitarios, usar el método típico de compilación out-of-source de cmake:

``` bash
user@siplasplas: $ mkdir build && cd build
user@siplasplas/build: $ cmake ..
user@siplasplas/build: $ cmake --build .
```

*Si vais a trabajar con varias entornos a la vez es recomendable tener una carpeta de build por entorno. Por ejemplo, cuando trabajo en windows suelo probar las cosas con Visual Studio, MinGW (GCC), y Cygwin (GCC y Clang), con una carpeta de build para cada uno:*

``` bash
user@siplasplas: $ mkdir buildVS && cd buildVS
user@siplasplas/buildVS: $ cmake ..
user@siplasplas: $ mkdir buildMinGW && cd buildMinGW
user@siplasplas/buildMinGW: $ cmake ..
cygwin@siplasplas: $ mkdir buildCygwin && cd buildCygwin
cygwin@siplasplas/buildCygwin: $ cmake ..
```

*También se puede escribir un projecto de cmake que haga de "paraguas", de modo que configure todos los projectos que necesiteis. Podéis ver un ejemplo [aquí](https://github.com/Manu343726/cpp-dod-tests).*

Aunque podéis correr los tests unitarios directamente lanzando los ejecutables, están añadidos a la configuración de [ctest](https://cmake.org/Wiki/CMake/Testing_With_CTest) del proyecto:

``` bash
user@siplasplas/build: $ ctest . -VV
```

*El generador de Visual Studio es multi-configuración, así que tendréis que decirle a CTest qué configuración (Release o Debug) queréis testear:*

``` bash
user@siplasplas/build: $ ctest -C Debug . -VV
```

*Además tener en cuenta que GTest y GMock se compilan para la configuración especificada en tiempo de configuración (Es decir, la variable `${CMAKE_BUILD_TYPE}`), así que aseguraros de haber configurado el proyecto en el modo correspondiente antes de lanzar el test, si no no enlazará.*

### Reflexión

El motor de reflexión mostrado durante el curso utiliza un script Python para analizar y generar automáticamente el código extra necesario para la reflexión, además dicho script utiliza la API libclang para analizar el código fuente.
Por ello, para compilar y ejecutar los ejemplos de reflexión es necesario tener instalados Python 2.7 y libclang, así como instalar las dependencias del script:

1. **Instalar Python:**

    En linux os debería bastar con instalarlo a través del package manager de turno. En Windows podéis descargarlo desde [aquí](https://www.python.org/downloads/). Es muy importante que os aseguréis que tanto el intérprete como los scripts se añaden al `PATH`. En el caso de Windows con la configuración por defecto `C:\Python27` y `C:\Python27\scripts`.

    Además es necesario instalar `pip`, el gestor de dependencias de python. El método típico es descargar el script [`get-pip.py`](https://bootstrap.pypa.io/get-pip.py) y ejecutarlo:

    ``` shell
$ python get-pip.py
```

2. **Instalar libclang:**

 - Windows: La versión compilada de Clang incluye libclang, debería funcionaros sin problemas nada más instalarlo. Si no es así, probablemente sea un problema con el path de `libclang.so`. Tanto el script del parser como la interfaz CMake incluyen un flag por el que pasar la ruta a `libclang.so` por si esto ocurre. Podéis descargar Clang para windows desde [aquí](http://llvm.org/releases/download.html).
 - Linux: Dependiendo de la distro el paquete se llamará de una manera u otra. En debian por ejemplo, el que se está usando en las builds de Travis, es `libclang1-[VERSION CLANG]`. Se suele incluir como parte del paquete correspondiente a Clang.

3. **Instalar dependencias:**

    El script incluye un archivo `requirements.txt` con las dependencias de éste:
    
``` shell
$ pip install -r include/reflection/parser/requirements.txt
```

### 3rdParty/cmake

Es un pequeño repo donde he ido subiendo utilidades para hacer más llevadero el uso de CMake. Por ahora consta de:

 - `exec-targets.cmake`: Define una función `exec_target(NAME <nombre> PREFIX <prefijo> [COMPILE_OPTIONS <opciones de compilación>])` que crea un target ejecutable de nombre `<prefijo>_<nombre>` a partir del archivo `<prefijo>s/<nombre>.cpp`. Por ejemplo:
 
 ``` cmake
 exec_target(NAME patata PREFIX example)
 ```
  
  crea un ejecutable `example_patata` a partir del archivo `examples/patata.cpp`. 
  Para poder utilizar dicha función es necesario llamar a `setup_exec_targets([PROJECT <nombre del proyecto>] ...)` con la configuración que queréis usar (Source folder, include directory, tests directory, etc). De todos modos está diseñado para projectos con la organización mencionada arriba, así que una llamada sin parámetros debería funcionar perfectamente.

 - `gmock.cmake`: Descarga y configura el framework de mocking y unit testing de google ([Google Test Framework](https://github.com/google/googletest)), de forma que sea muy sencillo utilizarlo para escribir nuestros propios tests. Consta de dos funciones `install_gtestgmock([VERSION <versión>] ...)` (Descarga y configura el framework para nuestro proyecto) y `gmock_test_target(NAME <nombre>)` (Un wrapper de `exec_target()` que configura un ejecutable de test y lo enlaza automáticamente con GTest/GMock).
 - `vs_source_groups.cmake`: El generador de Visual Studio no tiene en cuenta la estructura de carpetas de los sources/includes de un target a la hora de crear el proyecto correspondiente. Esto es muy molesto ya que significa que normalmente solo aparecen los `.cpp`s y sin respetar nuestra estructura de carpetas. La función `generate_vs_source_groups(<nombre grupo> <top directory>)` configura el proyecto de visual studio para que muestre todos los archivos, siguiendo la estructura de carpetas original. Se llama automáticamente desde `exec_target()` si se está usando Visual Studio.
 - [`cmakepp.cmake`](https://github.com/toeb/cmakepp): Un framework que extiende CMake con un montón de características, utilidades, etc. Aunque es complejo y falta bastante documentación Tobias es bastante majo, si le preguntáis suele contestar muy detalladamente. *He copiado la version compilada de cmakepp para poder modificar algunas cosas, por eso no es un submódulo.*
 - `custom_properties.cmake`: Tanto los targets como los sources en un proyecto CMake tienen una serie de propiedades asociadas, que nosotros podemos consultar, modificar, etc. Por desgracia el conjunto de propiedades es fijo, no podemos añadir propias. Éste módulo implementa una serie de funciones similares a la interfaz de propiedades de CMake para definir nuestras propias propiedades para archivos y targets.

## Licencia

El contenido aquí compartido está publicado bajo la licencia MIT.

