cmake_minimum_required(VERSION 2.8)

project(reports)

set(SOURCES
    BasicTextReport.cpp
    BasicTextReport.hpp
    ReportBase.hpp
)

add_library(reports ${SOURCES})
