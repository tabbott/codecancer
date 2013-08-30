set(TEST_LIBS index ${LIBDIVSUFSORT_LIBRARIES})

add_unit_tests(TestIndexLib
    TestBwt.cpp
    TestCloneHunter.cpp
    TestLcpArray.cpp
    TestSourceIndex.cpp
    TestSuffixArray.cpp
)
