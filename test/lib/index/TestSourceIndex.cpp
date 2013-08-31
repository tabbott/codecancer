#include "index/SourceIndex.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class TestSourceIndex : public ::testing::Test{
public:
    void SetUp() {
        src1 << "abc";
        src2 << "de";
        src3 << "fg";

        sidx.addSource("src/1", src1);
        sidx.addSource("src/2", src2);
        sidx.addSource("src/3", src3);
    }

protected:
    stringstream src1;
    stringstream src2;
    stringstream src3;
    SourceIndex sidx;
};

TEST_F(TestSourceIndex, addSource) {
    EXPECT_EQ(7u, sidx.size());
    EXPECT_EQ("abcdefg", sidx.string());
    stringstream lol("hij");
    sidx.addSource("lol", lol);
    EXPECT_EQ(10u, sidx.size());
    EXPECT_EQ("abcdefghij", sidx.string());
}

TEST_F(TestSourceIndex, fileAtOffset) {
    SourceFile result;
    for (size_t i = 0; i < 3; ++i) {
        result = sidx.fileAtOffset(i);
        EXPECT_EQ("src/1", result.name)
            << "Wrong file at byte " << i;
        EXPECT_EQ(i, result.globalToFileOffset(i))
            << "Wrong offset at byte " << i;
    }

    for (size_t i = 3; i < 5; ++i) {
        result = sidx.fileAtOffset(i);
        EXPECT_EQ("src/2", result.name)
            << "Wrong file at byte " << i;
        EXPECT_EQ(i - 3, result.globalToFileOffset(i))
            << "Wrong offset at byte " << i;
    }

    for (size_t i = 5; i < 7; ++i) {
        result = sidx.fileAtOffset(i);
        EXPECT_EQ("src/3", result.name)
            << "Wrong file at byte " << i;
        EXPECT_EQ(i - 5, result.globalToFileOffset(i))
            << "Wrong offset at byte " << i;
    }

    EXPECT_THROW(sidx.fileAtOffset(7), std::runtime_error);
}

TEST_F(TestSourceIndex, emptyFileThrows) {
    stringstream mt;
    EXPECT_THROW(sidx.addSource("empty", mt), std::runtime_error);
}

TEST_F(TestSourceIndex, lcpOutOfBounds) {
    stringstream mt;
    EXPECT_THROW(sidx.longestCommonPrefix(100, 150), std::runtime_error);
}

TEST(SourceIndex, lcp) {
    SourceIndex sidx;
    stringstream ss("banana");
    sidx.addSource("x", ss);

    EXPECT_EQ(3u, sidx.longestCommonPrefix(1, 3));
    EXPECT_EQ(2u, sidx.longestCommonPrefix(2, 4));
    EXPECT_EQ(1u, sidx.longestCommonPrefix(3, 5));
    EXPECT_EQ(0u, sidx.longestCommonPrefix(0, 1));
}

TEST(SourceIndex, spaces) {
    SourceIndex sidx;
    stringstream ss1("1 2 3 ");
    stringstream ss2(" 4 5 6");

    sidx.addSource("x", ss1);
    sidx.addSource("y", ss2);

    EXPECT_EQ("1 2 3  4 5 6", sidx.string());
}
