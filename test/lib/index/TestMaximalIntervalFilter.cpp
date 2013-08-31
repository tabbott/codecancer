#include "index/MaximalIntervalFilter.hpp"
#include "index/LcpArray.hpp"
#include "index/SuffixArray.hpp"
#include "index/SourceIndex.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

template<typename T>
class TestMaximalIntervalFilter : public ::testing::Test {
public:
    void SetUp() {
        text = "This frog. This freeze. That frog. That freeze.";
        std::stringstream ss(text);
        sidx.addSource("x", ss);
        sa = makeSuffixArray<T>(sidx.string());
        makeLcpArray(sidx, sa, std::back_inserter(lcp));
    }

protected:
    std::string text;
    SourceIndex sidx;
    std::vector<T> sa;
    std::vector<T> lcp;
};

typedef ::testing::Types<uint32_t, uint64_t> IntTypes;
TYPED_TEST_CASE(TestMaximalIntervalFilter, IntTypes);

TYPED_TEST(TestMaximalIntervalFilter, observe) {
    std::vector<LcpInterval> intervals;
    // These expected intervals were validated by hand.
    auto expectedIntervals = std::vector<LcpInterval>{
        {8, 3, 4},
        {9, 5, 6},
        {9, 8, 9},
        {7, 13, 14},
        };

    std::set<std::string> duplicates;
    std::set<std::string> expectedDuplicates{
        " freeze.",
        " frog. Th",
        ". That fr",
        "This fr",
        };
    std::function<void(LcpInterval const&)> cb = [&]
        (LcpInterval const& interval) {
            intervals.push_back(interval);
            size_t pos = this->sa[interval.leftBound];
            duplicates.insert(this->text.substr(pos, interval.lcp));
        };

    typedef std::vector<TypeParam> SuffixArray;
    MaximalIntervalFilter<SuffixArray> filter(5, this->sa, this->text, cb);
    visitLcpIntervals(this->lcp, filter);
    EXPECT_EQ(expectedIntervals, intervals);
    EXPECT_EQ(expectedDuplicates, duplicates);
}
