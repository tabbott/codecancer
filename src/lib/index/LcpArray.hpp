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
    T lastValue;

    lcpData[0] = 0;
    sa.foreach(
        [&] (size_t idx, T value) {
            if (idx == 0) {
                lcpData[idx] = 0;
            }
            else {
                lcpData[idx] = si.longestCommonPrefix(value, lastValue);
            }

            lastValue = value;
        });

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
