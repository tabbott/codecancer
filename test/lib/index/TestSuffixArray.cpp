#include "index/SuffixArray.hpp"

#include <gtest/gtest.h>

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
    EXPECT_EQ(1u, sa.decimation());
    EXPECT_EQ(6u, sa.size());

    EXPECT_EQ(5u, sa[0]);
    EXPECT_EQ(3u, sa[1]);
    EXPECT_EQ(1u, sa[2]);
    EXPECT_EQ(0u, sa[3]);
    EXPECT_EQ(4u, sa[4]);
    EXPECT_EQ(2u, sa[5]);
}

TYPED_TEST(TestSuffixArray, decimate) {
    string input("abcdefghijklmnopqrstuvwxyz");
    SuffixArray<TypeParam> sa(input);
    SuffixArray<TypeParam> saCopy(sa);

    EXPECT_EQ(26u, sa.size());
    sa.decimate(5);
    EXPECT_EQ(5u, sa.decimation());
    EXPECT_THROW(sa.decimate(2), std::runtime_error);
    EXPECT_EQ(6u, sa.size());

    EXPECT_EQ(0u, sa[0]);
    EXPECT_EQ(5u, sa[1]);
    EXPECT_EQ(10u, sa[2]);
    EXPECT_EQ(15u, sa[3]);
    EXPECT_EQ(20u, sa[4]);
    EXPECT_EQ(25u, sa[5]);

    saCopy.decimate(25);
    EXPECT_EQ(25u, saCopy.decimation());
    EXPECT_THROW(saCopy.decimate(2), std::runtime_error);
    EXPECT_EQ(2u, saCopy.size());
    EXPECT_EQ(0u, saCopy[0]);
    EXPECT_EQ(25u, saCopy[1]);
}

TYPED_TEST(TestSuffixArray, fileIo) {
    string input("this is not a string");
    SuffixArray<TypeParam> sa(input);
    EXPECT_EQ(input.size(), sa.size());

    stringstream out;
    sa.toStream(out);
    string s = out.str();
    EXPECT_EQ((1 + input.size()) * sizeof(TypeParam), s.size());

    SuffixArray<TypeParam> saCopy = SuffixArray<TypeParam>::fromStream(out);

    EXPECT_EQ(sa, saCopy);
}
