cmake_minimum_required(VERSION 2.8)

project(reports)

set(SOURCES
    CpdXmlReport.cpp
    CpdXmlReport.hpp
    BasicTextReport.cpp
    BasicTextReport.hpp
    ReportBase.cpp
    ReportBase.hpp
)

add_library(reports ${SOURCES})
