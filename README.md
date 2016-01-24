# siplasplas [![Build Status](https://travis-ci.org/GueimUCM/siplasplas.svg?branch=master)](https://travis-ci.org/GueimUCM/siplasplas) [![Build status](https://ci.appveyor.com/api/projects/status/1rf604ixu7drh89t?svg=true)](https://ci.appveyor.com/project/Manu343726/siplasplas)

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

Dado que el curso cubre C++14, es necesario un compilador con soporte al menos parcial de C++14. Recomiendo utilizar un compilador cuanto más nuevo mejor. Está probada sobre lod digueintes compiladores:

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
