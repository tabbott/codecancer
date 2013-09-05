#pragma once

#include "SourceIndex.hpp"
#include "SuffixArray.hpp"
#include "LcpInterval.hpp"

#include <cassert>
#include <stack>
#include <string>
#include <vector>

// There are more far efficient ways to do this. We'll see how slow this is
// for the genome codebase and optimize if needed.
template<typename SuffixArray, typename OutputIterator>
void makeLcpArray(
        SourceIndex const& si,
        SuffixArray const& sa,
        OutputIterator out)
{
    *out++ = 0;

    for (size_t idx = 1; idx < sa.size(); ++idx) {
        *out++ = si.longestCommonPrefix(sa[idx], sa[idx - 1]);
    }
}

template<typename ArrayType, typename Function>
void visitLcpIntervals(ArrayType const& lcps, Function f) {
    std::stack<LcpInterval> intervals;

    size_t leftBound(0);

    intervals.emplace(LcpInterval{0, 0, 0});
    for (size_t i = 1; i < lcps.size(); ++i) {
        leftBound = i - 1;

        assert(!intervals.empty());
        while (lcps[i] < intervals.top().lcp) {
            intervals.top().rightBound = i - 1;
            f(intervals.top());
            leftBound = intervals.top().leftBound;
            intervals.pop();
        }
        if (lcps[i] > intervals.top().lcp)
            intervals.emplace(LcpInterval{lcps[i], leftBound, 0});
    }

    intervals.top().rightBound = lcps.size() - 1;
    f(intervals.top());
}
