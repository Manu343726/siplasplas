---
layout: standardese-doc
---

  - [SIPLASPLAS\_ASSERT](assert.md#SIPLASPLAS_ASSERT "SIPLASPLAS_ASSERT") - When `NDEBUG` is not defined, this macro is defined to instance a cpp::AssertExpression object. Else it instances a cpp::DummyAssertExpression object which does nothing.

  - [SIPLASPLAS\_ASSERT\_BE](assert.md#SIPLASPLAS_ASSERT_BE "SIPLASPLAS_ASSERT_BE") - The assertion success if \\p a is bigger or equal to \\p b. Fails otherwise. The comparison is done by means of `operator>=(a, b)`.

  - [SIPLASPLAS\_ASSERT\_BT](assert.md#SIPLASPLAS_ASSERT_BT "SIPLASPLAS_ASSERT_BT") - The assertion success if \\p a is bigger than \\p b. Fails otherwise. The comparison is done by means of `operator>(a, b)`.

  - [SIPLASPLAS\_ASSERT\_COMP\_IMPL](assert.md#SIPLASPLAS_ASSERT_COMP_IMPL "SIPLASPLAS_ASSERT_COMP_IMPL") - This macro builds a detailed assertion expression for expressions that compare two values using an specific comparison operator. The resulting diagnostic message is of the form:

  - [SIPLASPLAS\_ASSERT\_EQ](assert.md#SIPLASPLAS_ASSERT_EQ "SIPLASPLAS_ASSERT_EQ") - The assertion success if \\p a and \\p b are equal. Fails otherwise. Equality is tested using `operator==(a, b)`.

  - [SIPLASPLAS\_ASSERT\_FALSE](assert.md#SIPLASPLAS_ASSERT_FALSE "SIPLASPLAS_ASSERT_FALSE") - The assertion success if the expression evaluates to false. Fails otherwise.

  - [SIPLASPLAS\_ASSERT\_LE](assert.md#SIPLASPLAS_ASSERT_LE "SIPLASPLAS_ASSERT_LE") - The assertion success if \\p a is less or equal to \\p b. Fails otherwise. Comparison is done by means of `operator<=(a, b)`.

  - [SIPLASPLAS\_ASSERT\_LT](assert.md#SIPLASPLAS_ASSERT_LT "SIPLASPLAS_ASSERT_LT") - The assertion success if \\p a is less than \\p b. Fails otherwise. Comparison is done by means of `operator<(a, b)`.

  - [SIPLASPLAS\_ASSERT\_NE](assert.md#SIPLASPLAS_ASSERT_NE "SIPLASPLAS_ASSERT_NE") - The assertion success if \\p a and \\p b are not equal. Fails otherwise. Inequality is tested using `operator!=(a, b)`.

  - [SIPLASPLAS\_ASSERT\_TRUE](assert.md#SIPLASPLAS_ASSERT_TRUE "SIPLASPLAS_ASSERT_TRUE") - The assertion success if the expression evaluates to true. Fails otherwise.

  - [SIPLASPLAS\_UTILITY\_COMPILES](compiles.md#SIPLASPLAS_UTILITY_COMPILES "SIPLASPLAS_UTILITY_COMPILES") - This macro hides the syntax complexity of cpp::compiles() and checks directly whatever expression is passed as argument to the macro:

  - `cpp`
    
      - [AssertException](assert.md#cpp::AssertException "cpp::AssertException") - An AssertException is thrown whenever a siplasplas assertion fails (See AssertExpression). AssertException::what() returns a detailed message about the assertion failure.
    
      - [AssertExpression](assert.md#cpp::AssertExpression "cpp::AssertExpression") - Siplasplas implements assertions as instances of AssertExpression class. An assert expression object takes the result (true or false) of the assertion, its source location (file, line) and a detailed message. The assertion is evaluated during the destruction of the assertion object. If the assertion failed (i.e. **the value of the result was false**) the AssertExpression destructor throws an AssertException exception.
    
      - [DummyAssertExpression](assert.md#cpp::DummyAssertExpression "cpp::DummyAssertExpression") - When siplasplas assertions are disabled DummyAssertExpression objects are instanced instead, and the expression does nothing.
    
      - [DynamicLibrary](dynamiclibrary.md#cpp::DynamicLibrary "cpp::DynamicLibrary") - DynamicLibrary is a RAII abstraction over a OS-specific shared library handle. The library handle is refcounted and automatically released when there are no more DynamicLibrary instances referencing it. Symbols can be loaded from the library using getSymbol() function.
    
      - [Exception\<Ex, Base\>](exception.md#cpp::Exception\<Ex,%20Base\> "cpp::Exception\<Ex, Base\>") - Declares a siplasplas exception type.
    
      - [Hash\<T\>](hash.md#cpp::Hash\<T\> "cpp::Hash\<T\>") - This template provides the features of cpp::hash() as a functor template, suitable for unordered containers.
    
      - [Identity](identity.md#cpp::Identity "cpp::Identity") - The identity function takes a value of any type and returns it as is. The function performs no mutation of the value. Given an expression `x` `decltype(x) == decltype(Identity()(x))` and the yield value is the same. This function is useful to delay the evaluation of an expression to the second template processing phase (The on instantiation phase). See cpp::staticIf or cpp::compiles.
    
      - [RawHash\<T\>](hash.md#cpp::RawHash\<T\> "cpp::RawHash\<T\>") - This template provides the features of cpp::raw\_hash() as a functor template, suitable for unordered containers.
    
      - [Throw\<Exception, Args...\>(const std::string &,Args &&...)](exception.md#cpp::Throw\<Exception,%20Args...\>\(const%20std::string%20&,Args%20&&...\) "cpp::Throw\<Exception, Args...\>(const std::string &,Args &&...)") - Throws an exception. See cpp::exception().
    
      - [UniversalReference\<T, IsLvalueReference, IsConst\>](universal_reference.md#cpp::UniversalReference\<T,%20IsLvalueReference,%20IsConst\> "cpp::UniversalReference\<T, IsLvalueReference, IsConst\>") - This template and its specialization provides a common interface to store references to lvalue and rvalues. UniversalReference has different spacializations for each value category supported (reference to lvalue, const reference to lvalue, rvalue). Instances of this types should be created through cpp::universalReference() function, which instances the appropiate template given the input value category.
    
      - [UniversalReference\<
        T,
        false,
        false
        \>](universal_reference.md#cpp::UniversalReference\<
        T,
        false,
        false
        \> "cpp::UniversalReference\<
        T,
        false,
        false
        \>") - See cpp::UniversalReference main template documentation for details
    
      - [UniversalReference\<
        T,
        true,
        false
        \>](universal_reference.md#cpp::UniversalReference\<
        T,
        true,
        false
        \> "cpp::UniversalReference\<
        T,
        true,
        false
        \>") - See cpp::UniversalReference main template documentation for details
    
      - [compiles\<Expr\>(Expr &&)](compiles.md#cpp::compiles\<Expr\>\(Expr%20&&\) "cpp::compiles\<Expr\>(Expr &&)") - This function takes an expression in the form of an SFINAE-friendly context and checks if the expression can be compiled. For example:
    
      - [construct\<T, Args...\>(T \*,Args &&...)](destroy.md#cpp::construct\<T,%20Args...\>\(T%20*,Args%20&&...\) "cpp::construct\<T, Args...\>(T *,Args &&...)") - This function performs an in-place construction of an object of type T in the given address. Arguments are passed as-is to the object constructor. The behavior is undefined if `alignment(pointer) != alignof(T)`.
    
      - [construct\<T, Args...\>(void \*,Args &&...)](destroy.md#cpp::construct\<T,%20Args...\>\(void%20*,Args%20&&...\) "cpp::construct\<T, Args...\>(void *,Args &&...)") - This function performs an in-place construction of an object of type T in the given address. Arguments are passed as-is to the object constructor. The behavior is undefined if `alignment(pointer) != alignof(T)`.
    
      - [exceptionSkippingFrames\<Exception, Args...\>(std::size\_t,const std::string &,Args &&...)](exception.md#cpp::exceptionSkippingFrames\<Exception,%20Args...\>\(std::size_t,const%20std::string%20&,Args%20&&...\) "cpp::exceptionSkippingFrames\<Exception, Args...\>(std::size_t,const std::string &,Args &&...)") - This function creates an exception object of the given type using a custom parameterized error message (See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList)). Exceptions instanced throught this function are reported automatically (See cpp::logError() and cpp::logException()).
    
      - [hash\<T, U, Args...\>(const T &,const U &,const Args &...)](hash.md#cpp::hash\<T,%20U,%20Args...\>\(const%20T%20&,const%20U%20&,const%20Args%20&...\) "cpp::hash\<T, U, Args...\>(const T &,const U &,const Args &...)") - This function is a generalization of unary cpp::hash() that accepts two or more input values. The resulting hash code is computed as the hash combination (See cpp::hash\_combine()) of the first value and the hash combination of the rest:
    
      - [hash\_combine\<T\>(std::size\_t,const T &)](hash.md#cpp::hash_combine\<T\>\(std::size_t,const%20T%20&\) "cpp::hash_combine\<T\>(std::size_t,const T &)") - Literally copied from [this stack overflow thread](http://stackoverflow.com/questions/7110301/generic-hash-for-tuples-in-unordered-map-unordered-set) which in turn got it from [`boost::hash_combine()`](http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html).
    
      - [lexical\_cast\<T\>(const T &)](lexical_cast.md#cpp::lexical_cast\<T\>\(const%20T%20&\) "cpp::lexical_cast\<T\>(const T &)") - This function invokes `operator<<(std::ostream&, const T&)` to get an string representation of a given value.
    
      - [lexical\_cast\<T\>(const std::string &)](lexical_cast.md#cpp::lexical_cast\<T\>\(const%20std::string%20&\) "cpp::lexical_cast\<T\>(const std::string &)") - This function invokes `operator>>(std::istream&, T&)` to assign a T value from an string representation of it.
    
      - [logException\<Ex, Message\>(const Message &,std::size\_t)](exception.md#cpp::logException\<Ex,%20Message\>\(const%20Message%20&,std::size_t\) "cpp::logException\<Ex, Message\>(const Message &,std::size_t)") - This function logs an error report (See cpp::logError()) about a thrown exception. The error report is of the form:
    
      - [logException\<Ex\>(const Ex &,std::size\_t)](exception.md#cpp::logException\<Ex\>\(const%20Ex%20&,std::size_t\) "cpp::logException\<Ex\>(const Ex &,std::size_t)") - This function logs a detailed error report from an exception object. See cpp::logError(). The error report is of the form:
    
      - [raw\_hash\<T\>(const T &)](hash.md#cpp::raw_hash\<T\>\(const%20T%20&\) "cpp::raw_hash\<T\>(const T &)") - This function ignores the `std::hash` specialization of the value type and implements a bytewise hash value instead. Bytewise hash is computed as a hash combination of each byte of the value storage, in the range `[addressof(value), addressof(value) + sizeof(T))`. The value is copyed to an intermediary aligned storage to perform the byte traversal.
    
      - [staticIf\<Condition, ThenBody, Args...\>(const ThenBody &,Args &&...)](staticif.md#cpp::staticIf\<Condition,%20ThenBody,%20Args...\>\(const%20ThenBody%20&,Args%20&&...\) "cpp::staticIf\<Condition, ThenBody, Args...\>(const ThenBody &,Args &&...)") - An static conditional allows to conditionally evaluate some code depending on the value of a compile time property. The body of the conditional is implemented by user provided functions.
    
      - [universalReference\<T\>(T &&)](universal_reference.md#cpp::universalReference\<T\>\(T%20&&\) "cpp::universalReference\<T\>(T &&)") - This function checks the value category of the given value and instances the apropiate cpp::UniversalReference specialization. Note the returned type depends on the value category. For an alternative with common a type, see cpp::typeerasure::AnyArg

  - `cpp::detail`
    
      - [AlignedMallocAlingOffset](memory_manip.md#cpp::detail::AlignedMallocAlingOffset "cpp::detail::AlignedMallocAlingOffset") - This type limits the maximum alignment requirement that can be passed to aligned\_malloc() stores the distance to the start of the allocated block so it can be deallocated in aligned\_free(). To use as little extra memory as possible, a 8 bit unsigned integer is used by default, which means up to 256 byte alignment boundary is supported by default. Users can change that limit by defining `SIPLASPLAS_UTILITY_ALIGNEDMALLOC_ALIGNOFFSET_BITS` to the with in bits of the unsigned integer used for offset storage (8, 16, 32, and 64 are supported)
    
      - [If\<Condition\>](staticif.md#cpp::detail::If\<Condition\> "cpp::detail::If\<Condition\>") - Implements the then branch of an static conditional.
    
      - [aligned\_malloc(std::size\_t,std::size\_t,std::size\_t)](memory_manip.md#cpp::detail::aligned_malloc\(std::size_t,std::size_t,std::size_t\) "cpp::detail::aligned_malloc(std::size_t,std::size_t,std::size_t)") - This function allocates a memory block starting at a specific alignment boundary. Users can also set some extra bytes for bookeeping data before the returned block. To deallocate blocks allocated with aligned\_malloc(), use aligned\_free(), never std::free()
    
      - [aligned\_malloc\_block(void \*,std::size\_t)](memory_manip.md#cpp::detail::aligned_malloc_block\(void%20*,std::size_t\) "cpp::detail::aligned_malloc_block(void *,std::size_t)") - aligned\_malloc() allocates an oversized block in order to acomplish the alignment requirements While aligned\_malloc() returns the expected aligned block, this function can be used to get the complete allocated block.
    
      - [aligned\_ptr(char \*,std::size\_t)](memory_manip.md#cpp::detail::aligned_ptr\(char%20*,std::size_t\) "cpp::detail::aligned_ptr(char *,std::size_t)")
    
      - [aligned\_ptr(const char \*,std::size\_t)](memory_manip.md#cpp::detail::aligned_ptr\(const%20char%20*,std::size_t\) "cpp::detail::aligned_ptr(const char *,std::size_t)")
    
      - [aligned\_ptr(const void \*,std::size\_t)](memory_manip.md#cpp::detail::aligned_ptr\(const%20void%20*,std::size_t\) "cpp::detail::aligned_ptr(const void *,std::size_t)")
    
      - [aligned\_ptr(void \*,std::size\_t)](memory_manip.md#cpp::detail::aligned_ptr\(void%20*,std::size_t\) "cpp::detail::aligned_ptr(void *,std::size_t)")
    
      - [is\_aligned(char \*,std::size\_t)](memory_manip.md#cpp::detail::is_aligned\(char%20*,std::size_t\) "cpp::detail::is_aligned(char *,std::size_t)")
    
      - [is\_aligned(const char \*,std::size\_t)](memory_manip.md#cpp::detail::is_aligned\(const%20char%20*,std::size_t\) "cpp::detail::is_aligned(const char *,std::size_t)")
    
      - [is\_aligned(const void \*,std::size\_t)](memory_manip.md#cpp::detail::is_aligned\(const%20void%20*,std::size_t\) "cpp::detail::is_aligned(const void *,std::size_t)")
    
      - [is\_aligned(void \*,std::size\_t)](memory_manip.md#cpp::detail::is_aligned\(void%20*,std::size_t\) "cpp::detail::is_aligned(void *,std::size_t)")
    
      - [missalignment(char \*,std::size\_t)](memory_manip.md#cpp::detail::missalignment\(char%20*,std::size_t\) "cpp::detail::missalignment(char *,std::size_t)")
    
      - [missalignment(const char \*,std::size\_t)](memory_manip.md#cpp::detail::missalignment\(const%20char%20*,std::size_t\) "cpp::detail::missalignment(const char *,std::size_t)")
    
      - [missalignment(const void \*,std::size\_t)](memory_manip.md#cpp::detail::missalignment\(const%20void%20*,std::size_t\) "cpp::detail::missalignment(const void *,std::size_t)")
    
      - [missalignment(void \*,std::size\_t)](memory_manip.md#cpp::detail::missalignment\(void%20*,std::size_t\) "cpp::detail::missalignment(void *,std::size_t)")
    
      - [readTaggedPointer\<R, Args...\>(R (\*)(Args...))](memory_manip.md#cpp::detail::readTaggedPointer\<R,%20Args...\>\(R%20\(*\)\(Args...\)\) "cpp::detail::readTaggedPointer\<R, Args...\>(R (*)(Args...))") - Assuming the pointer is a tagged pointer, this function reads the data tagged in the 16 more significative bits of the pointer. Compilation fails if this function is used in non 64 bit architectures.
    
      - [readTaggedPointer\<T\>(T \*)](memory_manip.md#cpp::detail::readTaggedPointer\<T\>\(T%20*\) "cpp::detail::readTaggedPointer\<T\>(T *)") - Assuming the pointer is a tagged pointer, this function reads the data tagged in the 16 more significative bits of the pointer. Compilation fails if this function is used in non 64 bit architectures.
    
      - [tagPointer\<R, U, Args...\>(R (\*)(Args...),U)](memory_manip.md#cpp::detail::tagPointer\<R,%20U,%20Args...\>\(R%20\(*\)\(Args...\),U\) "cpp::detail::tagPointer\<R, U, Args...\>(R (*)(Args...),U)") - This function uses the tagged pointer technique to store data in a 64 bit virtual memory address. Passing data of more than 16 bits wide has undefined behavior. Compilation fails if this function is used in non 64 bit architectures. Note accessing a tagged pointer directly may cause a segmentation fault. See cpp::untagPointer().
    
      - [tagPointer\<T, U\>(T \*,U)](memory_manip.md#cpp::detail::tagPointer\<T,%20U\>\(T%20*,U\) "cpp::detail::tagPointer\<T, U\>(T *,U)") - This function uses the tagged pointer technique to store data in a 64 bit virtual memory address. Passing data of more than 16 bits wide has undefined behavior. Compilation fails if this function is used in non 64 bit architectures. Note accessing a tagged pointer directly may cause a segmentation fault. See cpp::untagPointer().
    
      - [untagPointer\<R, Args...\>(R (\*)(Args...))](memory_manip.md#cpp::detail::untagPointer\<R,%20Args...\>\(R%20\(*\)\(Args...\)\) "cpp::detail::untagPointer\<R, Args...\>(R (*)(Args...))") - Assuming the pointer is a tagged pointer, this function removes the tagged data and returns the memory address ready to be referenced. Compilation fails if this function is used in non 64 bit architectures.
    
      - [untagPointer\<T\>(T \*)](memory_manip.md#cpp::detail::untagPointer\<T\>\(T%20*\) "cpp::detail::untagPointer\<T\>(T *)") - Assuming the pointer is a tagged pointer, this function removes the tagged data and returns the memory address ready to be referenced. Compilation fails if this function is used in non 64 bit architectures.

  - `cpp::utility`
    
      - [errorLogger()](error_logger.md#cpp::utility::errorLogger\(\) "cpp::utility::errorLogger()") - This function returns a logger pointing to an isolated siplasplas errors log file. All exceptions and assertion failures are logged throught this logger.
    
      - [logError\<Message, Args...\>(const Message &,Args &&...)](error_logger.md#cpp::utility::logError\<Message,%20Args...\>\(const%20Message%20&,Args%20&&...\) "cpp::utility::logError\<Message, Args...\>(const Message &,Args &&...)") - Effectively calls `logErrorSkippingFrames(1, message, args...)`.
    
      - [logErrorSkippingFrames\<Message, Args...\>(std::size\_t,const Message &,Args &&...)](error_logger.md#cpp::utility::logErrorSkippingFrames\<Message,%20Args...\>\(std::size_t,const%20Message%20&,Args%20&&...\) "cpp::utility::logErrorSkippingFrames\<Message, Args...\>(std::size_t,const Message &,Args &&...)") - This function logs a detailed error report with a stack trace ending at the caller function. Also supports skipping the last \\p framesToSkip stack frames from the trace in case the trace is invoked from an API with multiple implementation layers. The generated report follows the format:
