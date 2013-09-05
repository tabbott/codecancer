cmake_minimum_required(VERSION 2.8)

# Build/configure rapidxml

include(ExternalProject)
set_directory_properties(PROPERTIES
    EP_PREFIX ${CMAKE_BINARY_DIR}/vendor)

set(RAPIDXML_ROOT ${CMAKE_BINARY_DIR}/vendor/rapidxml)
ExternalProject_Add(
    rapidxml-1.13
    URL ${CMAKE_CURRENT_SOURCE_DIR}/vendor/rapidxml-1.13.tar.gz
    PATCH_COMMAND patch -Np1 < ${CMAKE_CURRENT_SOURCE_DIR}/vendor/rapidxml-gcc.patch
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    SOURCE_DIR "${RAPIDXML_ROOT}"
)

include_directories(${RAPIDXML_ROOT})
message("RAPIDXML_ROOT set to ${RAPIDXML_ROOT}")
