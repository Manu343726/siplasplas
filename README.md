# siplasplas [![Build Status](https://travis-ci.org/GueimUCM/siplasplas.svg?branch=master)](https://travis-ci.org/GueimUCM/siplasplas)

Ejemplos y utilidades del curso de C++ avanzado de la asociación GUEIM, Universidad Complutense de Madrid.

## Contenido

El repo se organiza como una pequeña biblioteca con las diferentes estructuras de datos, ejemplos, y otras utilidades que veamos a lo largo del curso. La idea es mostrar como se suele organizar un proyecto C++ open source (Build system, testing, CI, etc) a la vez que se hostean los contenidos dados en clase.

Consta de los siguientes contenidos:

 - **`include/`**: Cabezeras de las distintas utilidades, ejemplos, etc. 
 - **`src/`**: Implementación.
 - **`examples/`**: Ejemplos de uso.
 - **`tests/`**: Tests unitarios.
 - **`3rdParty/`**: Dependencias externas.

### Setup

Solo hay que clonar el repo e inicializar los submódulos de las dependencias externas:

``` bash
$ git clone https://github.com/GueimUCM/siplasplas
$ cd siplasplas
$ git submodule update --init
```

Para compilar los ejemplos y los test unitarios, usar el método típico de compilación out-of-source de cmake:

``` bash
user@siplasplas: $ mkdir build && cd build
user@siplasplas/build: $ cmake ..
user@siplasplas/build: $ cmake --build .
```

Aunque podéis correr los tests unitarios directamente lanzando los ejecutables, están añadidos a la configuración de ctest del proyecto:

``` bash
user@siplasplas/build: $ ctest . -VV
```

### 3rdParty/cmake

Es un pequeño repo donde he ido subiendo utilidades para hacer más llevadero el uso de CMake. Por ahora consta de:

 - `exec-targets.cmake`: Define una función `exec_target(NAME <nombre> PREFIX <prefijo> [COMPILE_OPTIONS <opciones de compilación>])` que crea un target ejecutable de nombre `<prefijo>_<nombre>` a partir del archivo `<prefijo>s/<nombre>.cpp`. Por ejemplo:
 
 ``` cmake
 exec_target(NAME patata PREFIX example)
 ```
  
  crea un ejecutable `example_patata` a partir del archivo `examples/patata.cpp`. 
  Para poder utilizar dicha función es necesario llamar a `setup_exec_targets([PROJECT <nombre del proyecto>] ...)` con la configuración que queréis usar (Source folder, include directory, tests directory, etc). De todos modos está diseñado para projectos con la organización mencionada arriba, así que una llamada sin parámetros debería funcionar perfectamente.

 - `gmock.cmake`: Descarga y configura el framework de unit testing de google ([Google Test Framework](https://github.com/google/googletest)), de forma que sea muy sencillo utilizarlo para escribir nuestros propios tests. Consta de dos funciones `install_gtestgmock([VERSION <versión>] ...)` (Descarga y configura el framework para nuestro proyecto) y `gmock_test_target(NAME <nombre>)` (Un wrapper de `exec_target()` que configura un ejecutable de test y lo enlaza automáticamente con GTest/GMock).
 - `vs_source_groups.cmake`: El generador de Visual Studio no tiene en cuenta la estructura de carpetas de los sources/includes de un target a la hora de crear el proyecto correspondiente. Esto es muy molesto ya que significa que normalmente solo aparecen los `.cpp`s y sin respetar nuestra estructura de carpetas. La función `generate_vs_source_groups(<nombre grupo> <top directory>)` configura el proyecto de visual studio para que muestre todos los archivos, siguiendo la estructura de carpetas original. Se llama automáticamente desde `exec_target()` si se está usando Visual Studio.

## Licencia

El contenido aquí compartido está publicado bajo la licencia MIT.
