#include "index/Bwt.hpp"
#include "index/SuffixArray.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <functional>
#include <sstream>
#include <string>

using namespace std;

template<typename T>
class TestBwt : public ::testing::Test {
};

template<typename IntType, size_t N>
struct TestInstance {
    typedef IntType int_type;
    static size_t size() { return N; }
};

// Test with both 32 bit and 64 bit integers in various sizes
typedef ::testing::Types<uint32_t, uint64_t> BwtTestTypes;

TYPED_TEST_CASE(TestBwt, BwtTestTypes);

TYPED_TEST(TestBwt, constructBwt) {
    string input("banana");
    SuffixArray<TypeParam> sa(input);

    std::string output;
    size_t pos = bwtFromSa(input, sa, output);

    EXPECT_EQ(4u, pos);
    EXPECT_EQ("annbaa", output);
}
