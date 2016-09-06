if(SIPLASPLAS_IMGUI_SFML_EXAMPLES_ENABLED)
    add_siplasplas_example_simple(imgui-sfml            EXCLUDE_FROM_RUN_ALL DEPENDS imgui-sfml)
    add_siplasplas_example_simple(imgui-nodegrapheditor EXCLUDE_FROM_RUN_ALL DEPENDS imgui-sfml)
endif()
