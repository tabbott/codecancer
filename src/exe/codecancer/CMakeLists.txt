cmake_minimum_required(VERSION 2.8)

set(SOURCES
    main.cpp
)

add_executable(codecancer main.cpp)
target_link_libraries(codecancer index
    ${Boost_LIBRARIES} ${LIBDIVSUFSORT_LIBRARIES})
