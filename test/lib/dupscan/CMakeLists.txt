set(TEST_LIBS dupscan ${LIBDIVSUFSORT_LIBRARIES})

add_unit_tests(TestDupScanLib
    TestCloneHunter.cpp
    TestSourceIndex.cpp
    TestLcpArray.cpp
)
