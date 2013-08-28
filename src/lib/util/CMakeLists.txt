cmake_minimum_required(VERSION 2.8)

project(util)

set(SOURCES
    TempFile.cpp
    TempFile.hpp
    Timer.hpp
)

add_library(util ${SOURCES})
target_link_libraries(util ${Boost_LIBRARIES})
