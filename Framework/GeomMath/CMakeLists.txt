# TODO: add windows's configs
SET(GEOMMATH_LIB_FILE ${CMAKE_CURRENT_BINARY_DIR}/libGeomMath.a)
SET(ISPC_COMPILER ispc)
SET(ISPC_COMPILER_PATH ${PROJECT_SOURCE_DIR}/External/Darwin/ispc-release/bin/)
SET(ISPC_OPTIONS --arch=x86-64 --target=host)
SET(LIBRARIAN_COMMAND ar -cr ${GEOMMATH_LIB_FILE})

# cmake now officially support ispc
# TODO: update ispc configs in this file in modern-cmake's way.

SET(GEOMMATH_LIB_HEADER_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(GeomMath STATIC IMPORTED GLOBAL)
add_dependencies(GeomMath GeomMathLib)

set_target_properties(GeomMath
        PROPERTIES
        IMPORTED_LOCATION ${GEOMMATH_LIB_FILE}
        INTERFACE_INCLUDE_DIRECTORIES ${GEOMMATH_LIB_HEADER_FOLDER}
        )

add_subdirectory(ispc)
add_subdirectory(test)