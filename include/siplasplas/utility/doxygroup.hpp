/**
 * \defgroup utility
 * \brief Some random utilities needed by other siplasplas
 * modules.
 *
 * The siplaspals-utility module implements various utilities
 * used in the implementation of other siplasplas modules.
 * These utilities are not grouped in any special way, but we
 * encourage the policy of rising utilities into full-fledged modules
 * if these became too complex.
 *
 * \todo Move meta.hpp, function_traits.hpp, tuple.hpp, and fusion.hpp to a
 * siplasplas-meta module.
 * \todo Move error_logger.hpp and exception.hpp to a siplasplas-exception
 * module?
 * \todo Move cpp::Overload<> to function_traits.hpp
 *
 * siplasplas-utility includes the following features:
 *
 *  - **Assertion macros**: A set of assertion macros based in Google
 *  Test syntax. See assert.hpp.
 *  - **Clonable pointer**: A smart pointer implementing clone semantics.
 *  See cloning_ptr.hpp.
 *  - **Dynamic library loading**: Cross platform classes to load symbols from
 *  shared libraries at runtime. See cpp::DynamicLibrary.
 *  - **Exceptions hierarchy**: An exception hierarchy with [fmt](https://github.com/fmtlib/fmt)
 *  syntax for error messages and automatic logging of exception stack traces. See exception.hpp.
 *  - **Function type introspection utilities**: Metafunctions to check function types and signatures.
 *  See function_traits.hpp.
 *  - **One header metaprogramming library**: A header full of metaprogramming tools. See meta.hpp.
 *  - **Overloaded functor factory**: See function.hpp.
 *  - **Heterogeneous operations on type and parameter packs**: See fusion.hpp.
 *  - **Hash utilities**: A cpp::hash() function and equivalent functors that give
 *  default hashing for enum types, tuples, function pointer types, etc. See hash.hpp.
 *  - **Preprocessor utilities**: See preprocessor.hpp.
 *  - **String utilities**: See string.hpp.
 *  - **Raw memory manipulation**: Tools for raw memory manipulation such as functions to compute aligned
 *  addresses, functions to read/write values directly into raw memory, etc. See memory_manip.hpp.
 *  - **Tuple manipulation tools**: See tuple.hpp.
 */
