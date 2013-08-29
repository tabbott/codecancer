#pragma once

#include "SourceIndex.hpp"
#include "SuffixArray.hpp"

#include <vector>

// There are more far efficient ways to do this. We'll see how slow this is
// for the genome codebase and optimize if needed.
template<typename T>
std::vector<T> makeLcpArray(
        SourceIndex const& si,
        SuffixArray<T> const& sa)
{
    std::vector<T> lcpData(sa.size());

    lcpData[0] = 0;
    for (size_t i = 1; i < sa.size(); ++i) {
        lcpData[i] = si.longestCommonPrefix(sa[i], sa[i-1]);
    }

    return lcpData;
}

template<typename T>
class LcpTree {
public:
    typedef T value_type;

    struct Node {
        value_type leftBound;
        value_type rightBound;
        value_type lcp;
    };
};
