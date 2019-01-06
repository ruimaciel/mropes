find_package(Threads REQUIRED)


add_library(check STATIC IMPORTED )

set_target_properties(check
    PROPERTIES
    IMPORTED_LOCATION "/usr/local/lib/libcheck.a"
    INTERFACE_INCLUDE_DIRECTORIES "/usr/local/include"
)

include_directories(${PROJECT_SOURCE_DIR})

set(mropes_tests_prefix "check_mropes_")

file(GLOB mropes_test_files
    "${CMAKE_CURRENT_SOURCE_DIR}/${mropes_tests_prefix}*.c"
    )


foreach(mropes_test_file ${mropes_test_files})

    get_filename_component(mropes_test_filename ${mropes_test_file} NAME_WE)
    string(REGEX REPLACE "^${mropes_tests_prefix}" "" mropes_test_name ${mropes_test_filename} )

    add_executable(${mropes_test_name} ${mropes_test_file})

    target_link_libraries(${mropes_test_name}
        check
        m rt
        ${CMAKE_THREAD_LIBS_INIT}
        mropes
        )

    add_test(NAME ${mropes_test_name}
        COMMAND ${mropes_test_name}
        )

endforeach(mropes_test_file)

