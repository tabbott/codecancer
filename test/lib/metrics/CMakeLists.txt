set(TEST_LIBS metrics ${LIBDIVSUFSORT_LIBRARIES} jsoncpp)

add_unit_tests(TestMetricsLib
    TestDuplicateByteCounter.cpp
)
