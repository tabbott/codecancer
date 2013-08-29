cmake_minimum_required(VERSION 2.8)

project(index)

set(SOURCES
    SourceFileList.cpp
    SourceFileList.hpp
    SourceIndex.hpp
    SuffixArray.hpp
    prefix.hpp
)

add_library(index ${SOURCES})
