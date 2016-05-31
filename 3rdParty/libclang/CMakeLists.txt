
function(clang_package LIBCLANG_VERSION SOURCE PACKAGE EXTENSION)
    # The following variables list the postfixes of LLVM + Clang packages
    # from the official LLVM download page. The format of the download url
    # is "http://llvm.org/releases/${CLANG_VERSION}/clang+llvm-${CLANG_VERSION}-${ARCH}-${PLATFORM}${FILE_EXT}"
    # (Except windows releases, that currently don't have clang).
    #
    # siplasplas currently supports Windows and Linux only, with a limited
    # set of architectures (x86_64 only). This is not strictly a limitation
    # of siplasplas but the most common platforms we decided to support first. Feel
    # free to open an issue requesting new platforms.
    #
    # Note for x86_64 linux we're using the Ubuntu 14.04 package.

    if(WIN32 OR CYGWIN)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(${SOURCE} "http://llvm.org/releases/${LIBCLANG_VERSION}/" PARENT_SCOPE)
            set(${PACKAGE} "LLVM-${LIBCLANG_VERSION}-win64" PARENT_SCOPE)
            set(${EXTENSION} ".exe" PARENT_SCOPE)
        else()
            message(FATAL_ERROR "libclang package not found")
        endif()
    else()
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(${SOURCE} "http://llvm.org/releases/${LIBCLANG_VERSION}/" PARENT_SCOPE)
            set(${PACKAGE} "clang+llvm-${LIBCLANG_VERSION}-x86_64-linux-gnu-ubuntu-14.04" PARENT_SCOPE)
            set(${EXTENSION} ".tar.xz" PARENT_SCOPE)
        else()
            message(FATAL_ERROR "libclang package not found")
        endif()
    endif()
endfunction()

if(NOT SIPLASPLAS_LIBCLANG_VERSION)
    message(FATAL_ERROR "libclang version not set")
endif()

clang_package(${SIPLASPLAS_LIBCLANG_VERSION} SOURCE PACKAGE EXTENSION)

add_siplasplas_thirdparty(libclang
URL
    "${SOURCE}${PACKAGE}${EXTENSION}"
)
