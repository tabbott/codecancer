cmake_minimum_required(VERSION 2.8)

# Build/configure jsoncpp

include(ExternalProject)
set_directory_properties(PROPERTIES
    EP_PREFIX ${CMAKE_BINARY_DIR}/vendor)

set(JSONCPP_ROOT ${CMAKE_BINARY_DIR}/vendor/jsoncpp)
ExternalProject_Add(
    jsoncppufsort-0.6.0
    URL ${CMAKE_CURRENT_SOURCE_DIR}/vendor/jsoncpp-src-0.6.0-rc2.tar.gz
    PATCH_COMMAND patch -Np1 < ${CMAKE_CURRENT_SOURCE_DIR}/vendor/jsoncpp-cmake.patch
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${JSONCPP_ROOT}
    )
include_directories(${JSONCPP_ROOT}/include)
message("JSONCPP_ROOT set to ${JSONCPP_ROOT}")

set(JSONCPP_LIBRARY
    ${JSONCPP_ROOT}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}jsoncpp${CMAKE_STATIC_LIBRARY_SUFFIX}
)

add_library(jsoncpp STATIC IMPORTED)
set_property(TARGET jsoncpp PROPERTY IMPORTED_LOCATION ${JSONCPP_LIBRARY})
