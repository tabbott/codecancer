cmake_minimum_required(VERSION 2.8)

project(index)

set(SOURCES
    Bwt.hpp
    CloneHunter.hpp
    MaximalIntervalFilter.hpp
    LcpArray.cpp
    LcpArray.hpp
    prefix.hpp
    SourceFileList.cpp
    SourceFileList.hpp
    SourceIndex.hpp
    SuffixArray.hpp
)

add_library(index ${SOURCES})
