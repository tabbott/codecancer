cmake_minimum_required(VERSION 2.8)

project(util)

set(SOURCES
    Sequence.hpp
    StreamHandler.hpp
    TempFile.cpp
    TempFile.hpp
    Timer.hpp
    UnsignedVectorView.hpp
)

add_library(util ${SOURCES})
target_link_libraries(util ${Boost_LIBRARIES})
