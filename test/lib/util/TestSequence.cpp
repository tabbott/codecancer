#include "util/Sequence.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(TestSequence, foreachIndexIf) {
    std::string x("\n1\n23\n456\n");
    std::vector<size_t> pos;
    foreachIndexIf(x.begin(), x.end(),
        [&pos] (size_t idx) { pos.push_back(idx); },
        [] (char c) { return c == '\n'; });

    auto expected = std::vector<size_t>{0, 2, 5, 9};
    EXPECT_EQ(expected, pos);
}

TEST(TestSequence, commonPrefix) {
    std::string x("123 abc");
    std::string y("123 xyz");
    EXPECT_EQ(4u, commonPrefix(x.begin(), x.end(), y.begin(), y.end()));
}
