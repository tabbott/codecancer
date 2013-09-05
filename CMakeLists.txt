cmake_minimum_required(VERSION 2.8)

project(codecancer)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "/usr")
endif()
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/vendor;${CMAKE_SOURCE_DIR}/build-common/cmake")
include(TestHelper)
include(VersionHelper)
include(ProjectHelper)

set(EXE_VERSION_SUFFIX ${FULL_VERSION})

# Dump clang compilation database for use with YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
configure_file(.ycm_extra_conf.py.in ${CMAKE_SOURCE_DIR}/.ycm_extra_conf.py @ONLY)

#configure_file(version/version.h.in version/version.h @ONLY)
#include_directories(${PROJECT_BINARY_DIR}/version)

find_package(Boost 1.48 REQUIRED filesystem iostreams program_options serialization system chrono regex)
link_libraries(rt)
include_directories(${Boost_INCLUDE_DIRS})


set(CMAKE_C_FLAGS "-std=c99 -Wall -ansi -pedantic")
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -pedantic -Wno-long-long -Wno-variadic-macros")

include(divsufsort)
include(jsoncpp)

# make sure to pick up headers from library dirs
include_directories("src/lib")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
enable_testing(true)
add_projects(src/lib)
add_projects(src/exe)
add_projects(test/lib)


#include(PackageInfo.cmake)
