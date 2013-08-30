#include "index/LcpArray.hpp"
#include "index/SourceIndex.hpp"
#include "index/SuffixArray.hpp"

#include <gtest/gtest.h>

#include <cstdint>
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
        sidx.addSource("x", ss);
        sa.reset(new SuffixArray<T>(sidx.string()));
    }

    SourceIndex sidx;
    stringstream ss;
    std::unique_ptr<SuffixArray<T>> sa;
};

typedef ::testing::Types<uint32_t, uint64_t> IntTypes;
TYPED_TEST_CASE(TestLcpArray, IntTypes);

TYPED_TEST(TestLcpArray, build) {
    auto lcp = makeLcpArray(this->sidx, *this->sa);
    auto expected = vector<TypeParam>{0, 2, 1, 1, 2, 0, 0, 1};
    EXPECT_EQ(expected, lcp);
}

TYPED_TEST(TestLcpArray, visitLcpIntervals) {
    auto lcp = makeLcpArray(this->sidx, *this->sa);
    auto expected = vector<LcpInterval>{
        LcpInterval{2, 0, 1},
        LcpInterval{2, 3, 4},
        LcpInterval{1, 0, 4},
        LcpInterval{1, 6, 7},
        };

    vector<LcpInterval> observed;

    visitLcpIntervals(lcp,
        [&observed] (LcpInterval x) {
            observed.push_back(x);
            cout << x << "\n";
        });

    this->sa->printSuffixes(cout, this->sidx.string());

    EXPECT_EQ(expected, observed);
}

TYPED_TEST(TestLcpArray, visitLcpIntervalTreeNodes) {
    auto lcp = makeLcpArray(this->sidx, *this->sa);
    auto expected = vector<LcpInterval>{
        LcpInterval{2, 0, 1},
        LcpInterval{2, 3, 4},
        LcpInterval{1, 0, 4, { LcpInterval{2, 0, 1}, LcpInterval{2, 3, 4} }},
        LcpInterval{1, 6, 7},
        };

    vector<LcpInterval> observed;

    visitLcpIntervalTreeNodes(lcp,
        [&observed] (LcpInterval x) {
            observed.push_back(x);
            x.print(cout);
        });

    this->sa->printSuffixes(cout, this->sidx.string());

    EXPECT_EQ(expected, observed);
}
