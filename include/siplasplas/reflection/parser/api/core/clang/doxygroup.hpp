/**
 * \ingroup core
 * \defgroup clang
 * \brief A C++ interface to the libclang C API
 *
 * This module defines a C++ object oriented API to access to
 * the libclang API. The API features automatic management of
 * libclang resources by wrapping libclang handles in RAII classes,
 * and the API is designed to operate with C++ datatypes (Strings, vectors,
 * etc).
 *
 * The API tries to have as low overhead as possible, by just forwarding calls
 * to libclang. All properties of libclang entities, such as the spelling of a cursor
 * or translation unit, are represented as member functions of the entity class.
 * This classes do not chache any data but only act as an object oriented interface
 * to the raw libclang calls.
 */
