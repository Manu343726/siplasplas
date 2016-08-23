/**
 * \ingroup type-erasure
 * \defgroup features
 * \brief Generic implementation of type features
 *
 * This module provides implementation classes for the concepts defined in
 * `cpp::concepts` namespace. For each concept of name `C` defined in `cpp::concepts`
 * there's a `cpp::features::C` class that implements that concept.
 * The implementation works as follows:
 *
 *  - Feature classes have `apply()` template static methods with the signature needed
 *  to implement the concept.
 *
 *  - If the type given to `apply()` satisfies the concept, `apply()` invokes the
 *  default implementation of that concept. For example, if the concept represents to have
 *  a `push_back()` method, `apply()` will have the signature `apply<T, U>(T& object, U&& value)`
 *  and will invoke `object.push_back(std::forward<U>(value))`.
 *
 *  - If the type given to apply doesn't satisfy the concept, an exception is thrown
 */
