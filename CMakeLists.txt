cmake_minimum_required(VERSION 3.1)

project(mropes
    VERSION 0.0.5.1
    LANGUAGES C
    )

set(mropes_SOURCES
    src/rope.c
    src/rope_ops.c
    src/rope_stack.c
    src/version.h.in
    )

set(mropes_INCLUDES
    include/mropes/rope.h
    )

set(mropes_HEADERS
    src/rope_ops.h
    src/rope_stack.h
    )

configure_file(src/version.h.in include/mropes/version.h)

add_library(${PROJECT_NAME} SHARED
    ${mropes_SOURCES}
    ${mropes_INCLUDES}
    ${mropes_HEADERS}
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    C_STANDARD 90
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    SOVERSION 1
    PUBLIC_HEADER ${mropes_INCLUDES}
    )

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    LIBRARY DESTINATION lib/${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
    )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/include/mropes/version.h
  DESTINATION include/${PROJECT_NAME}
  )

install(
    EXPORT ${PROJECT_NAME}
    DESTINATION lib/cmake/${PROJECT_NAME}
    )

#Setup CPACK to generate installer
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_GENERATOR "DEB;TGZ;ZIP")
set(CPACK_DEBIAN_PACKAGE_NAME "mropes")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Rui Maciel")
include(CPack)

option(ENABLE_TESTS "Enable tests" ON)
if (${ENABLE_TESTS})
    enable_testing()
    add_subdirectory(tests)
endif()
