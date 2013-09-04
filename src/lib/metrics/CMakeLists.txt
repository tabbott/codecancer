cmake_minimum_required(VERSION 2.8)

project(metrics)

set(SOURCES
    DuplicateByteCounter.cpp
    DuplicateByteCounter.hpp
)

add_library(metrics ${SOURCES} ${Boost_LIBRARIES})
