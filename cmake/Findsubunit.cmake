find_library(SUBUNIT_LIBRARY
    NAMES subunit
    HINTS /usr /usr/local
    PATH_SUFFIXES lib
)

if(SUBUNIT_LIBRARY_NOT_FOUND)
endif()

if(NOT SUBUNIT_LIBRARY)
endif()

find_path(SUBUNIT_INCLUDE_DIR
    NAMES child.h
    PATH_SUFFIXES subunit
    )

add_library(subunit STATIC IMPORTED )

set_target_properties(subunit
    PROPERTIES
    IMPORTED_LOCATION ${SUBUNIT_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${SUBUNIT_INCLUDE_DIR}
)
