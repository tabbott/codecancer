#include "index/SourceIndexBuilder.hpp"
#include "index/SourceIndex.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class TestSourceIndexBuilder : public ::testing::Test{
public:
    void SetUp() {
        src1 << "abc\n123";
        src2 << "de\n45";
        src3 << "fg\n67";

        builder.addSource("src/1", src1);
        builder.addSource("src/2", src2);
        builder.addSource("src/3", src3);
    }

protected:
    stringstream src1;
    stringstream src2;
    stringstream src3;
    SourceIndexBuilder builder;
};

TEST_F(TestSourceIndexBuilder, build) {
    std::shared_ptr<SourceIndex> sidx = builder.build();
    EXPECT_EQ(17u, sidx->size());
    string data(sidx->data(), sidx->size());
    EXPECT_EQ(src1.str() + src2.str() + src3.str(), data);

    auto expectedFiles = std::vector<SourceFile>{
        {"src/1", 0, 6, {0, 4}},
        {"src/2", 7, 11, {0, 3}},
        {"src/3", 12, 16, {0, 3}}
        };

    EXPECT_EQ(expectedFiles, sidx->sourceFiles());
}
