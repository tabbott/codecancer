#pragma once

#include "LcpArray.hpp"
#include "SuffixArray.hpp"

#include <cstddef> // size_t
#include <string>
#include <functional>

template<typename SuffixArray>
struct MaximalIntervalFilter {
    typedef std::function<void(LcpInterval const&)> Reporter;

    MaximalIntervalFilter(size_t minLcp, SuffixArray const& sa, std::string const& text, Reporter& r)
        : _minLcp(minLcp)
        , _sa(sa)
        , _text(text)
        , _report(r)
    {
    }

    void operator()(LcpInterval const& interval) {
        if (wantInterval(interval)) {
            _report(interval);
        }
    }

    bool wantInterval(LcpInterval const& interval) const {
        if (interval.size() > 1 && interval.lcp < _minLcp)
            return false;

        for (size_t i = interval.leftBound; i < interval.rightBound; ++i) {
            if (!bwtEqual(i, i+1))
                return true;
        }

        return false;
    }

    bool bwtEqual(size_t x, size_t y) const {
        size_t saX = _sa[x];
        size_t saY = _sa[y];
        if (saX == 0 || saY == 0)
            return false;
        return _text[saX - 1] == _text[saY - 1];
    }

private:
    size_t _minLcp;
    SuffixArray const& _sa;
    std::string const& _text;
    Reporter& _report;
};
