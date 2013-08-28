cmake_minimum_required(VERSION 2.8)

set(SOURCES
    Deparser.cpp
    Deparser.hpp
)

add_library(perl_deparse ${SOURCES})
