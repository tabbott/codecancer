#include "metrics/DuplicateByteCounter.hpp"

#include <gtest/gtest.h>

#include <iostream>

class TestDuplicateByteCounter : public ::testing::Test {
public:
    DuplicateByteCounter counter;
};

TEST_F(TestDuplicateByteCounter, count) {
    counter.addInterval(100, 110);
    counter.addInterval(120, 130);
    counter.addInterval(125, 135);
    EXPECT_EQ(25u, counter.value());
}
