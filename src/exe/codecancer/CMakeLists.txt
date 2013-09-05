cmake_minimum_required(VERSION 2.8)

set(SOURCES
    main.cpp
)

add_executable(codecancer main.cpp)
target_link_libraries(codecancer ui index reports
    ${Boost_LIBRARIES} ${LIBDIVSUFSORT_LIBRARIES})
