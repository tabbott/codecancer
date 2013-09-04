#include "index/LcpArray.hpp"
#include "index/SourceIndex.hpp"
#include "index/SourceIndexBuilder.hpp"
#include "index/SuffixArray.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Input string for this test: aaacatat
// Sorted suffixes:
//     aaacatat
//     aacatat
//     acatat
//     at
//     atat
//     catat
//     t
//     tat

template<typename T>
class TestLcpArray : public ::testing::Test {
public:
    void SetUp() {
        ss << "aaacatat";

        SourceIndexBuilder builder;
        builder.addSource("x", ss);

        sidx = builder.build();

        sa = makeSuffixArray<T>(sidx->data(), sidx->size());

        lcp.reserve(sa.size());
        makeLcpArray(*sidx, sa, std::back_inserter(lcp));
    }

    std::shared_ptr<SourceIndex> sidx;
    stringstream ss;
    std::vector<T> sa;
    std::vector<T> lcp;
};

typedef ::testing::Types<uint32_t, uint64_t> IntTypes;
TYPED_TEST_CASE(TestLcpArray, IntTypes);

TYPED_TEST(TestLcpArray, build) {
    auto expected = vector<TypeParam>{0, 2, 1, 1, 2, 0, 0, 1};
    EXPECT_EQ(expected, this->lcp);
}

TYPED_TEST(TestLcpArray, visitLcpIntervals) {
    auto expected = vector<LcpInterval>{
        LcpInterval{2, 0, 1},
        LcpInterval{2, 3, 4},
        LcpInterval{1, 0, 4},
        LcpInterval{1, 6, 7},
        };

    vector<LcpInterval> observed;

    visitLcpIntervals(this->lcp,
        [&observed] (LcpInterval x) {
            observed.push_back(x);
            cout << x << "\n";
        });

    EXPECT_EQ(expected, observed);
}
