set(TEST_LIBS metrics ${LIBDIVSUFSORT_LIBRARIES})

add_unit_tests(TestMetricsLib
    TestDuplicateByteCounter.cpp
)
