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

    auto sa = makeSuffixArray<TypeParam>(input);
    auto expected = std::vector<TypeParam>{5, 3, 1, 0, 4, 2};

    EXPECT_EQ(expected, sa);
}
