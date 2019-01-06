cmake_minimum_required(VERSION 3.1)

project(mropes
    VERSION 0.0.1.0
    LANGUAGES C
    )

set(mropes_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mropes/rope.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mropes/rope_ops.c
    ${CMAKE_CURRENT_SOURCE_DIR}/mropes/rope_stack.c
    )

set(mropes_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/mropes/rope.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mropes/rope_ops.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mropes/rope_stack.h
    )

add_library(${PROJECT_NAME} SHARED
    ${mropes_SOURCES}
    ${mropes_HEADERS}
    )

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    C_STANDARD 90
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    )

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib/${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
    )

enable_testing()

add_subdirectory(tests)
