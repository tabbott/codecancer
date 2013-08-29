#include "index/SuffixArray.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <functional>
#include <sstream>
#include <string>

using namespace std;

template<typename T>
class TestSuffixArray : public ::testing::Test {
};

typedef ::testing::Types<uint32_t, uint64_t> SaTestTypes;
TYPED_TEST_CASE(TestSuffixArray, SaTestTypes);

TYPED_TEST(TestSuffixArray, buildsa) {
    string input("banana");
    SuffixArray<TypeParam> sa(input);
    EXPECT_EQ(6u, sa.size());

    EXPECT_EQ(5u, sa[0]);
    EXPECT_EQ(3u, sa[1]);
    EXPECT_EQ(1u, sa[2]);
    EXPECT_EQ(0u, sa[3]);
    EXPECT_EQ(4u, sa[4]);
    EXPECT_EQ(2u, sa[5]);
}

TYPED_TEST(TestSuffixArray, foreach) {
    string input("banana");
    SuffixArray<TypeParam> sa(input);
    EXPECT_EQ(6u, sa.size());

    struct Collector {
        std::vector<size_t> offsets;
        std::vector<TypeParam> values;

        void operator()(size_t idx, TypeParam value) {
            offsets.push_back(idx);
            values.push_back(value);
        }
    };

    std::vector<size_t> expectedOffsets{0, 1, 2, 3, 4, 5};
    std::vector<TypeParam> expectedValues{5, 3, 1, 0, 4, 2};

    Collector c;
    sa.foreach(std::ref(c));

    EXPECT_EQ(expectedOffsets, c.offsets);
    EXPECT_EQ(expectedValues, c.values);
}

