#include "index/EnhancedSuffixArray.hpp"

#include "index/SourceIndex.hpp"
#include "index/SourceIndexBuilder.hpp"
#include "util/TempFile.hpp"

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include <memory>
#include <sstream>
#include <string>


template<typename T>
class TestEnhancedSuffixArray : public ::testing::Test {
    void SetUp() {
        src1 << "ba";
        src2 << "na";
        src3 << "na";

        builder.addSource("src/1", src1);
        builder.addSource("src/2", src2);
        builder.addSource("src/3", src3);

        sidx = builder.build();
    }

protected:
    std::stringstream src1;
    std::stringstream src2;
    std::stringstream src3;
    SourceIndexBuilder builder;
    std::shared_ptr<SourceIndex> sidx;
};

TYPED_TEST_CASE_P(TestEnhancedSuffixArray);
typedef ::testing::Types<
    uint32_t, uint64_t
    > NumericTypes;

TYPED_TEST_CASE(TestEnhancedSuffixArray, NumericTypes);


TYPED_TEST(TestEnhancedSuffixArray, build) {
    auto tmpdir = TempDir::create(TempDir::CLEANUP);
    auto esa = EnhancedSuffixArray<TypeParam>::build(*this->sidx, tmpdir->path());
    ASSERT_EQ(6u, esa->sourceIndex().size());
    ASSERT_EQ(6u, esa->suffixArray().size());
    ASSERT_EQ(6u, esa->lcpArray().size());
    EXPECT_EQ("banana", std::string(esa->sourceIndex().data(), esa->sourceIndex().size()));

    // suf    sa  lcp
    // a        5   0
    // a$na     3   1
    // a$na$na  1   1
    // ba$na$na 0   0
    // na       4   0
    // na$na    2   2

    auto expectedSa = std::vector<TypeParam>{5, 3, 1, 0, 4, 2};
    auto expectedLcp = std::vector<TypeParam>{0, 1, 1, 0, 0, 2};

    auto const& sa = esa->suffixArray();
    auto const& lcp = esa->lcpArray();

    EXPECT_TRUE(std::equal(sa.data(), sa.data() + sa.size(), expectedSa.begin()));
    EXPECT_TRUE(std::equal(lcp.data(), lcp.data() + lcp.size(), expectedLcp.begin()));
}
