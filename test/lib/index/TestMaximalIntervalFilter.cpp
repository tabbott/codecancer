#include "index/MaximalIntervalFilter.hpp"
#include "index/LcpArray.hpp"
#include "index/SuffixArray.hpp"
#include "index/SourceIndex.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

template<typename T>
class TestMaximalIntervalFilter : public ::testing::Test {
public:
    void SetUp() {
        ss << "This frog. This freeze. That frog. That freeze.";
        sidx.addSource("x", ss);
        sa.reset(new SuffixArray<T>(sidx.string()));
        lcp = makeLcpArray(sidx, *sa);
    }

protected:
    std::stringstream ss;
    SourceIndex sidx;
    std::unique_ptr<SuffixArray<T>> sa;
    std::vector<T> lcp;
};

typedef ::testing::Types<uint32_t, uint64_t> IntTypes;
TYPED_TEST_CASE(TestMaximalIntervalFilter, IntTypes);

void helperBot(std::string const& s, size_t pos, size_t len) {
    std::cout << s.substr(0, pos) << "[" << s.substr(pos, len) << "]"
        << s.substr(pos+len);
}

TYPED_TEST(TestMaximalIntervalFilter, observe) {
    std::function<void(LcpInterval const&)> callback = [&]
        (LcpInterval const& interval) {
            std::cout << "I HAS A INTERVAL: " << interval << "\n";
            for (size_t i = interval.leftBound; i <= interval.rightBound; ++i) {
                size_t start = (*this->sa)[i];
                std::cout << "\t";
                helperBot(this->sidx.string(), start, interval.lcp);
                std::cout << "\n";
            }
            std::cout << "\n";
        };

    MaximalIntervalFilter<TypeParam> x(5, *this->sa, this->sidx.string(), callback);
    this->sa->printSuffixes(std::cout, this->sidx.string());

    visitLcpIntervals(this->lcp, x);
}
