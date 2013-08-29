set(TEST_LIBS index ${LIBDIVSUFSORT_LIBRARIES})

add_unit_tests(TestDupScanLib
    TestCloneHunter.cpp
    TestSourceIndex.cpp
    TestLcpArray.cpp
)
