cmake_minimum_required(VERSION 2.8)

project(index)

set(SOURCES
    LcpArray.hpp
    LcpInterval.cpp
    LcpInterval.hpp
    MaximalIntervalFilter.hpp
    SourceFile.cpp
    SourceFile.hpp
    SourceIndex.cpp
    SourceIndex.hpp
    SourceIndexBuilder.cpp
    SourceIndexBuilder.hpp
    SuffixArray.hpp
)

add_library(index ${SOURCES})
