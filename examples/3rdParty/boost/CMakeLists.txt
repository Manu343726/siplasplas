if(NOT MINGW AND SIPLASPLAS_BOOST_EXAMPLES) # See https://github.com/GueimUCM/siplasplas/issues/16
    # Boost examples
    add_siplasplas_example_simple(coroutine BOOST_COMPONENTS coroutine)
    if(MSVC)
        set_target_properties(examples-coroutine PROPERTIES LINK_FLAGS /SAFESEH:NO)
    endif()
endif()
