set(TEST_LIBS index ${LIBDIVSUFSORT_LIBRARIES})

add_unit_tests(TestIndexLib
    TestLcpArray.cpp
    TestSourceIndex.cpp
    TestSuffixArray.cpp
    TestMaximalIntervalFilter.cpp
)
