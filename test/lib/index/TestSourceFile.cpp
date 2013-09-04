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
            << " \"line_start_positions\": [[0, 5], [0, 6], [0, 7]]\n"
            << "}";
    }

protected:
    std::stringstream jsonIn;
};

TEST_F(TestSourceFile, fromJson) {
    auto sources = SourceFile::fromJson(jsonIn);
    std::vector<SourceFile> expected{
        {"a",  0,  9, {0, 5}},
        {"b", 10, 19, {0, 6}},
        {"c", 20, 29, {0, 7}}
        };

    EXPECT_EQ(expected, sources);
}
