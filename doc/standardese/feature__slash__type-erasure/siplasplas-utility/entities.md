---
layout: standardese-doc
---

  - [SIPLASPLAS\_ASSERT](assert.md#SIPLASPLAS_ASSERT "SIPLASPLAS_ASSERT") - When `NDEBUG` is not defined, this macro is defined to instance a cpp::AssertExpression object. Else it instances a cpp::DummyAssertExpression object which does nothing.

  - [SIPLASPLAS\_ASSERT\_BE](assert.md#SIPLASPLAS_ASSERT_BE "SIPLASPLAS_ASSERT_BE") - The assertion success if \\p a is bigger or equal to \\p b. Fails otherwise. The comparison is done by means of `operator>=(a, b)`.

  - [SIPLASPLAS\_ASSERT\_BT](assert.md#SIPLASPLAS_ASSERT_BT "SIPLASPLAS_ASSERT_BT") - The assertion success if \\p a is bigger than \\p b. Fails otherwise. The comparison is done by means of `operator>(a, b)`.

  - [SIPLASPLAS\_ASSERT\_COMP\_IMPL](assert.md#SIPLASPLAS_ASSERT_COMP_IMPL "SIPLASPLAS_ASSERT_COMP_IMPL") - This macro builds a detailed assertion expression for expressions that compare two values using an specific comparison operator. The resulting diagnostic message is of the form:

  - [SIPLASPLAS\_ASSERT\_EQ](assert.md#SIPLASPLAS_ASSERT_EQ "SIPLASPLAS_ASSERT_EQ") - The assertion success if \\p a and \\p b are equal. Fails otherwise. Equality is tested using `operator==(a, b)`.

  - [SIPLASPLAS\_ASSERT\_LE](assert.md#SIPLASPLAS_ASSERT_LE "SIPLASPLAS_ASSERT_LE") - The assertion success if \\p a is less or equal to \\p b. Fails otherwise. Comparison is done by means of `operator<=(a, b)`.

  - [SIPLASPLAS\_ASSERT\_LT](assert.md#SIPLASPLAS_ASSERT_LT "SIPLASPLAS_ASSERT_LT") - The assertion success if \\p a is less than \\p b. Fails otherwise. Comparison is done by means of `operator<(a, b)`.

  - [SIPLASPLAS\_ASSERT\_NE](assert.md#SIPLASPLAS_ASSERT_NE "SIPLASPLAS_ASSERT_NE") - The assertion success if \\p a and \\p b are not equal. Fails otherwise. Inequality is tested using `operator!=(a, b)`.

  - `cpp`
    
      - [AssertException](assert.md#cpp::AssertException "cpp::AssertException") - An AssertException is thrown whenever a siplasplas assertion fails (See AssertExpression). AssertException::what() returns a detailed message about the assertion failure.
    
      - [AssertExpression](assert.md#cpp::AssertExpression "cpp::AssertExpression") - Siplasplas implements assertions as instances of AssertExpression class. An assert expression object takes the result (true or false) of the assertion, its source location (file, line) and a detailed message. The assertion is evaluated during the destruction of the assertion object. If the assertion failed (i.e. **the value of the result was false**) the AssertExpression destructor throws an AssertException exception.
    
      - [DummyAssertExpression](assert.md#cpp::DummyAssertExpression "cpp::DummyAssertExpression") - When siplasplas assertions are disabled DummyAssertExpression objects are instanced instead, and the expression does nothing.
    
      - [DynamicLibrary](dynamiclibrary.md#cpp::DynamicLibrary "cpp::DynamicLibrary") - DynamicLibrary is a RAII abstraction over a OS-specific shared library handle. The library handle is refcounted and automatically released when there are no more DynamicLibrary instances referencing it. Symbols can be loaded from the library using getSymbol() function.
    
      - [Exception\<Ex, Base\>](exception.md#cpp::Exception\<Ex,%20Base\> "cpp::Exception\<Ex, Base\>") - Declares a siplasplas exception type.
    
      - [Hash\<T\>](hash.md#cpp::Hash\<T\> "cpp::Hash\<T\>") - This template provides the features of cpp::hash() as a functor template, suitable for unordered containers.
    
      - [RawHash\<T\>](hash.md#cpp::RawHash\<T\> "cpp::RawHash\<T\>") - This template provides the features of cpp::raw\_hash() as a functor template, suitable for unordered containers.
    
      - [Throw\<Exception, Args...\>(const std::string &,Args &&...)](exception.md#cpp::Throw\<Exception,%20Args...\>\(const%20std::string%20&,Args%20&&...\) "cpp::Throw\<Exception, Args...\>(const std::string &,Args &&...)") - Throws an exception. See cpp::exception().
    
      - [exceptionSkippingFrames\<Exception, Args...\>(std::size\_t,const std::string &,Args &&...)](exception.md#cpp::exceptionSkippingFrames\<Exception,%20Args...\>\(std::size_t,const%20std::string%20&,Args%20&&...\) "cpp::exceptionSkippingFrames\<Exception, Args...\>(std::size_t,const std::string &,Args &&...)") - This function creates an exception object of the given type using a custom parameterized error message (See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList)). Exceptions instanced throught this function are reported automatically (See cpp::logError() and cpp::logException()).
    
      - [hash\<T, U, Args...\>(const T &,const U &,const Args &...)](hash.md#cpp::hash\<T,%20U,%20Args...\>\(const%20T%20&,const%20U%20&,const%20Args%20&...\) "cpp::hash\<T, U, Args...\>(const T &,const U &,const Args &...)") - This function is a generalization of unary cpp::hash() that accepts two or more input values. The resulting hash code is computed as the hash combination (See cpp::hash\_combine()) of the first value and the hash combination of the rest:
    
      - [hash\_combine\<T\>(std::size\_t,const T &)](hash.md#cpp::hash_combine\<T\>\(std::size_t,const%20T%20&\) "cpp::hash_combine\<T\>(std::size_t,const T &)") - Literally copied from [this stack overflow thread](http://stackoverflow.com/questions/7110301/generic-hash-for-tuples-in-unordered-map-unordered-set) which in turn got it from [`boost::hash_combine()`](http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html).
    
      - [lexical\_cast\<T\>(const T &)](lexical_cast.md#cpp::lexical_cast\<T\>\(const%20T%20&\) "cpp::lexical_cast\<T\>(const T &)") - This function invokes `operator<<(std::ostream&, const T&)` to get an string representation of a given value.
    
      - [lexical\_cast\<T\>(const std::string &)](lexical_cast.md#cpp::lexical_cast\<T\>\(const%20std::string%20&\) "cpp::lexical_cast\<T\>(const std::string &)") - This function invokes `operator>>(std::istream&, T&)` to assign a T value from an string representation of it.
    
      - [logException\<Ex, Message\>(const Message &,std::size\_t)](exception.md#cpp::logException\<Ex,%20Message\>\(const%20Message%20&,std::size_t\) "cpp::logException\<Ex, Message\>(const Message &,std::size_t)") - This function logs an error report (See cpp::logError()) about a thrown exception. The error report is of the form:
    
      - [logException\<Ex\>(const Ex &,std::size\_t)](exception.md#cpp::logException\<Ex\>\(const%20Ex%20&,std::size_t\) "cpp::logException\<Ex\>(const Ex &,std::size_t)") - This function logs a detailed error report from an exception object. See cpp::logError(). The error report is of the form:
    
      - [raw\_hash\<T\>(const T &)](hash.md#cpp::raw_hash\<T\>\(const%20T%20&\) "cpp::raw_hash\<T\>(const T &)") - This function ignores the `std::hash` specialization of the value type and implements a bytewise hash value instead. Bytewise hash is computed as a hash combination of each byte of the value storage, in the range `[addressof(value), addressof(value) + sizeof(T))`. The value is copyed to an intermediary aligned storage to perform the byte traversal.
    
      - [staticIf\<Condition, ThenBody, Args...\>(const ThenBody &,Args &&...)](staticif.md#cpp::staticIf\<Condition,%20ThenBody,%20Args...\>\(const%20ThenBody%20&,Args%20&&...\) "cpp::staticIf\<Condition, ThenBody, Args...\>(const ThenBody &,Args &&...)") - An static conditional allows to conditionally evaluate some code depending on the value of a compile time property. The body of the conditional is implemented by user provided functions.

  - `cpp::detail`
    
      - [Identity](staticif.md#cpp::detail::Identity "cpp::detail::Identity") - A functor class implementing the identity function
    
      - [If\<Condition\>](staticif.md#cpp::detail::If\<Condition\> "cpp::detail::If\<Condition\>") - Implements the then branch of an static conditional.

  - `cpp::utility`
    
      - [errorLogger()](error_logger.md#cpp::utility::errorLogger\(\) "cpp::utility::errorLogger()") - This function returns a logger pointing to an isolated siplasplas errors log file. All exceptions and assertion failures are logged throught this logger.
    
      - [logError\<Message, Args...\>(const Message &,Args &&...)](error_logger.md#cpp::utility::logError\<Message,%20Args...\>\(const%20Message%20&,Args%20&&...\) "cpp::utility::logError\<Message, Args...\>(const Message &,Args &&...)") - Effectively calls `logErrorSkippingFrames(1, message, args...)`.
    
      - [logErrorSkippingFrames\<Message, Args...\>(int,const Message &,Args &&...)](error_logger.md#cpp::utility::logErrorSkippingFrames\<Message,%20Args...\>\(int,const%20Message%20&,Args%20&&...\) "cpp::utility::logErrorSkippingFrames\<Message, Args...\>(int,const Message &,Args &&...)") - This function logs a detailed error report with a stack trace ending at the caller function. Also supports skipping the last \\p framesToSkip stack frames from the trace in case the trace is invoked from an API with multiple implementation layers. The generated report follows the format:
