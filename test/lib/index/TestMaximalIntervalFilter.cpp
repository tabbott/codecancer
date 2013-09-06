#include "index/MaximalIntervalFilter.hpp"
#include "index/EnhancedSuffixArray.hpp"
#include "index/LcpArray.hpp"
#include "index/SuffixArray.hpp"
#include "index/SourceIndex.hpp"
#include "index/SourceIndexBuilder.hpp"
#include "util/TempFile.hpp"

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
        SourceIndexBuilder builder;
        builder.addSource("x", ss);
        sidx = builder.build();
        sa = makeSuffixArray<T>(sidx->data(), sidx->size());
        makeLcpArray(*sidx, sa, std::back_inserter(lcp));
    }

protected:
    std::string text;
    std::shared_ptr<SourceIndex> sidx;
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
    MaximalIntervalFilter<SuffixArray> filter(5, this->sa, *this->sidx, cb);
    visitLcpIntervals(this->lcp, filter);
    EXPECT_EQ(expectedIntervals, intervals);
    EXPECT_EQ(expectedDuplicates, duplicates);
}

template<typename T>
class TestMaximalIntervalFilterMulti : public ::testing::Test {
public:
    void SetUp() {
        src1 << "abc\n";
        src2 << "abcd\n";
        src3 << "abcde\n";

        SourceIndexBuilder builder;
        builder.addSource("src/1", src1);
        builder.addSource("src/2", src2);
        builder.addSource("src/3", src3);

        sidx = builder.build();
        tmpdir = TempDir::create(TempDir::CLEANUP);
        esa = EnhancedSuffixArray<T>::build(*sidx, tmpdir->path());

    }

protected:
    std::stringstream src1;
    std::stringstream src2;
    std::stringstream src3;
    TempDir::ptr tmpdir;
    std::string text;
    std::shared_ptr<SourceIndex> sidx;
    std::shared_ptr<EnhancedSuffixArray<T>> esa;
    std::vector<T> sa;
    std::vector<T> lcp;
};

TYPED_TEST_CASE(TestMaximalIntervalFilterMulti, IntTypes);

TYPED_TEST(TestMaximalIntervalFilterMulti, observe) {

    // The ESA for this test data, newlines are file boundaries and
    // are replaced with '+' below:
    // ---------------------------------------------
    // #Idx    SA      LCP     BWT     Suffix
    // 0       14      0       e       [+]
    //
    //   this repeat is not reported since it starts at file
    //   boundaries.
    //
    // 1       3       1       c       [+abcd+abcde+]
    // 2       8       1       d       [+abcde+]
    //
    //   the first two below are left maximal and should be reported as "abc"
    //   the second and third should be reported as abcd
    //   there was a bug where they were not being detected as maximal
    //   because the file boundaries were not respected.
    //
    // 3       0       0               [abc+abcd+abcde+]
    // 4       4       3       +       [abcd+abcde+]
    // 5       9       4       +       [abcde+]
    //
    // 6       1       0       a       [bc+abcd+abcde+]
    // 7       5       2       a       [bcd+abcde+]
    // 8       10      3       a       [bcde+]
    // 9       2       0       b       [c+abcd+abcde+]
    // 10      6       1       b       [cd+abcde+]
    // 11      11      2       b       [cde+]
    // 12      7       0       c       [d+abcde+]
    // 13      12      1       c       [de+]
    // 14      13      0       d       [e+]
    // ---------------------------------------------


    // uncomment this to print out the table above
    //std::cout << *this->esa << "\n";

    typedef typename EnhancedSuffixArray<TypeParam>::ArrayType SuffixArray;
    auto const& sa = this->esa->suffixArray();
    auto const& sidx = this->esa->sourceIndex();
    std::vector<LcpInterval> results;

    std::function<void(LcpInterval const&)> cb = [&]
        (LcpInterval const& interval) {
            results.push_back(interval);
        };

    MaximalIntervalFilter<SuffixArray> filter(3, sa, sidx, cb);
    visitLcpIntervals(this->esa->lcpArray(), filter);

    auto expected = std::vector<LcpInterval>{
        LcpInterval{4, 4, 5},
        LcpInterval{3, 3, 5},
        };

    EXPECT_EQ(expected, results);
}
