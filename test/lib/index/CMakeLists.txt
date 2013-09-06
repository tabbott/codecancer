set(TEST_LIBS index util ${LIBDIVSUFSORT_LIBRARIES} ${Boost_LIBRARIES} jsoncpp)

add_unit_tests(TestIndexLib
    TestDuplicationEventFactory.cpp
    TestEnhancedSuffixArray.cpp
    TestLcpArray.cpp
    TestSourceIndexBuilder.cpp
    TestSourceIndex.cpp
    TestSourceFile.cpp
    TestSuffixArray.cpp
    TestMaximalIntervalFilter.cpp
)
