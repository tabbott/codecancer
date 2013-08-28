
cmake_minimum_required(VERSION 2.8)

set(SOURCES
    SourceFileList.cpp
    SourceFileList.hpp
    SourceIndex.hpp
    SuffixArray.hpp
    prefix.hpp
)

add_library(dupscan ${SOURCES})
