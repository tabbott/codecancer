#include "index/SuffixArray.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <iterator>
#include <string>

using namespace std;

template<typename T>
class TestSuffixArray : public ::testing::Test {
};

typedef ::testing::Types<uint32_t, uint64_t> SaTestTypes;
TYPED_TEST_CASE(TestSuffixArray, SaTestTypes);

TYPED_TEST(TestSuffixArray, buildsa) {
    string input("banana");

    vector<TypeParam> sa = makeSuffixArray<TypeParam>(input);
    EXPECT_EQ(6u, sa.size());

    EXPECT_EQ(5u, sa[0]);
    EXPECT_EQ(3u, sa[1]);
    EXPECT_EQ(1u, sa[2]);
    EXPECT_EQ(0u, sa[3]);
    EXPECT_EQ(4u, sa[4]);
    EXPECT_EQ(2u, sa[5]);
}
