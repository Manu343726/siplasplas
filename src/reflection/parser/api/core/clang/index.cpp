#include "index.hpp"
#include <siplasplas/reflection/parser/api/core/clang/error.hpp>
#include <siplasplas/utility/assert.hpp>

using namespace ::cpp::reflection::parser::api::core::clang;

static constexpr bool CREATE_INDEX_EXCLUDE_DECLS_FROM_PCH = true;
static constexpr bool CREATE_INDEX_DISPLAY_DIAGNOSTICS = true;

Index::Index() :
    core::clang::CXIndex{::clang_createIndex(
        CREATE_INDEX_EXCLUDE_DECLS_FROM_PCH,
        CREATE_INDEX_DISPLAY_DIAGNOSTICS
    )}
{}

TranslationUnit Index::parse(
    const cpp::constexp::ConstStringView& filePath,
    const CompileOptions& compileOptions
)
{
    ::CXTranslationUnit rawTranslationUnit = nullptr;

    core::clang::Error error = ::clang_parseTranslationUnit2(
        rawHandle(),
        filePath.c_str(),
        compileOptions.options(),
        compileOptions.count(),
        nullptr, // No unsaved files
        0,       // No unsaved files
        0,       // Default options
        &rawTranslationUnit
    );

    if(!error.success())
    {
        SIPLASPLAS_ASSERT_TRUE(rawTranslationUnit == nullptr);
    }

    SIPLASPLAS_ASSERT_EQ(error.errorCode(), core::clang::Error::Code::Success)(
        "An error occurred when parsing translation unit '{}': {} (Details: '{}', compile options: {})",
        filePath,
        error.diagnostic(),
        error.details(),
        compileOptions
    ).noThrow();

    return {rawTranslationUnit};
}

TranslationUnit Index::parse(
    const cpp::constexp::ConstArrayView<core::clang::UnsavedFile>& unsavedFiles,
    const CompileOptions& compileOptions
)
{
    std::vector<::CXUnsavedFile> cxUnsavedFiles{unsavedFiles.size()};
    ::CXTranslationUnit rawTranslationUnit = nullptr;

    for(const auto& unsavedFile : unsavedFiles)
        cxUnsavedFiles.emplace_back(unsavedFile.cxUnsavedFile());


    core::clang::Error error = ::clang_parseTranslationUnit2(
        rawHandle(),
        unsavedFiles[0].fileName().c_str(),
        compileOptions.options(),
        compileOptions.count(),
        cxUnsavedFiles.data(),
        cxUnsavedFiles.size(),
        0,
        &rawTranslationUnit
    );

    if(!error.success())
    {
        SIPLASPLAS_ASSERT_TRUE(rawTranslationUnit == nullptr);
    }

    SIPLASPLAS_ASSERT_EQ(error.errorCode(), core::clang::Error::Code::Success)(
        "An error occurred when parsing translation unit '{}': (Details: '{}', unsaved files: {}, compile options: {})",
        error.diagnostic(),
        error.details(),
        unsavedFiles,
        compileOptions
    );

    return {rawTranslationUnit};
}
