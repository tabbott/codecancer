cmake_minimum_required(VERSION 2.8)

project(ui)

set(SOURCES
    BasicReportCommand.cpp
    BasicReportCommand.hpp
    CodeCancer.cpp
    CodeCancer.hpp
    CommandBase.cpp
    CommandBase.hpp
    IndexCommand.cpp
    IndexCommand.hpp
    ScanCommand.cpp
    ScanCommand.hpp
    XmlReportCommand.cpp
    XmlReportCommand.hpp
)

add_library(ui ${SOURCES} ${Boost_LIBRARIES})
