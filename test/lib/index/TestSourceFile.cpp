#include "index/SourceFile.hpp"

#include <gtest/gtest.h>

#include <sstream>

class TestSourceFile : public ::testing::Test {
public:
    void SetUp() {
        jsonIn << "{\n"
            << " \"filenames\": [\"a\", \"b\", \"c\"],\n"
            << " \"file_start_positions\": [0, 10, 20],\n"
            << " \"file_sizes\": [10, 10, 10],\n"
            << " \"line_start_positions\": [[0, 3, 7], [0, 6], [0, 7]]\n"
            << "}";
        sources = SourceFile::fromJson(jsonIn);
    }

protected:
    std::stringstream jsonIn;
    std::vector<SourceFile> sources;
};

TEST_F(TestSourceFile, fromJson) {
    std::vector<SourceFile> expected{
        {"a",  0,  9, {0, 3, 7}},
        {"b", 10, 19, {0, 6}},
        {"c", 20, 29, {0, 7}}
        };

    EXPECT_EQ(expected, sources);
}

TEST_F(TestSourceFile, lineNumbers) {
    auto const& file = sources[0];
    EXPECT_EQ(0u, file.lineNumberOfIndex(0));
    EXPECT_EQ(0u, file.lineNumberOfIndex(1));
    EXPECT_EQ(0u, file.lineNumberOfIndex(2));
    EXPECT_EQ(1u, file.lineNumberOfIndex(3));
    EXPECT_EQ(1u, file.lineNumberOfIndex(4));
    EXPECT_EQ(1u, file.lineNumberOfIndex(5));
    EXPECT_EQ(1u, file.lineNumberOfIndex(6));
    EXPECT_EQ(2u, file.lineNumberOfIndex(7));
    EXPECT_EQ(2u, file.lineNumberOfIndex(8));
    EXPECT_EQ(2u, file.lineNumberOfIndex(9));
}
