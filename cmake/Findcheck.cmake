find_library(CHECK_LIBRARY
    NAMES check
    HINTS /usr /usr/local
    PATH_SUFFIXES lib
)

if(CHECK_LIBRARY_NOT_FOUND)
endif()

if(NOT CHECK_LIBRARY)
endif()

find_path(CHECK_INCLUDE_DIR
    NAMES check.h
    )

add_library(check STATIC IMPORTED )

set_target_properties(check
    PROPERTIES
    IMPORTED_LOCATION ${CHECK_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${CHECK_INCLUDE_DIR}
)
