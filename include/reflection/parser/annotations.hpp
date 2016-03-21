#ifndef SIPLASPLAS_REFLECTION_PARSER_ANNOTATIONS_HPP
#define SIPLASPLAS_REFLECTION_PARSER_ANNOTATIONS_HPP

#include <siplasplas/utility/preprocessor.hpp>

#ifdef SIPLASPLAS_REFLECTION_RUNNING_DRLPARSER

// Named variadic macros are a GNU extension, but I don't care since this is
// only used when DRLParser is run
#define SIPLASPLAS_ANNOTATION(x...) __attribute__((annotate(#x)))
#define ANNOTATION_TEST(x...) SIPLASPLAS_ANNOTATION(x)
#else

#define SIPLASPLAS_ANNOTATION(...)
#define ANNOTATION_TEST(...)

#endif // SIPLASPLAS_REFLECTION_RUNNING_DRLPARSER

#define SIPLASPLAS_REFLECT SIPLASPLAS_ATTRIBUTE(reflection::enable_reflection)
#define SIPLASPLAS_NOREFLECT SIPLASPLAS_ATTRIBUTE(reflection::disable_reflection)

#endif // SIPLASPLAS_REFLECTION_PARSER_ANNOTATIONS_HPP
