cmake_minimum_required(VERSION 2.8)

project(ui)

set(SOURCES
    CodeCancer.cpp
    CodeCancer.hpp
    CommandBase.cpp
    CommandBase.hpp
    IndexCommand.cpp
    IndexCommand.hpp
    ScanCommand.cpp
    ScanCommand.hpp
)

add_library(ui ${SOURCES} ${Boost_LIBRARIES})
