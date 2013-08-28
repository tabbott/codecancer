#include "dupscan/LcpArray.hpp"
#include "dupscan/SourceIndex.hpp"
#include "dupscan/SuffixArray.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

TEST(TestLcpArray, build) {
    SourceIndex sidx;
    stringstream ss("aaacatat");
    sidx.addSource("x", ss);
    SuffixArray<uint32_t> sa(sidx.string());
    vector<uint32_t> lcp = makeLcpArray(sidx, sa);
    vector<uint32_t> expected{0, 2, 1, 1, 2, 0, 0, 1};

    for (size_t i = 0; i < sa.size(); ++i) {
        cout << sa[i] << "\t" << lcp[i] << "\t" << sidx.string().substr(sa[i]) << "\n";;
    }

    EXPECT_EQ(expected, lcp);
}

