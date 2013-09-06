set(TEST_LIBS index ${LIBDIVSUFSORT_LIBRARIES} ${Boost_LIBRARIES} jsoncpp)

add_unit_tests(TestIndexLib
    TestLcpArray.cpp
    TestSourceIndexBuilder.cpp
    TestSourceIndex.cpp
    TestSourceFile.cpp
    TestSuffixArray.cpp
    TestMaximalIntervalFilter.cpp
)
