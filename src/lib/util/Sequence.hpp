#pragma once

#include <cstddef>

template<typename InputIterType, typename Output, typename PredType>
void foreachIndexIf(
    InputIterType begin,
    InputIterType end,
    Output out,
    PredType pred)
{
    size_t idx(0);
    for (; begin != end; ++begin, ++idx) {
        if (pred(*begin))
            out(idx);
    }
}

template<typename IterTypeA, typename IterTypeB>
static size_t commonPrefix(
    IterTypeA aBeg, IterTypeA aEnd,
    IterTypeB bBeg, IterTypeB bEnd)
{
    size_t rv(0);
    while (aBeg != aEnd && bBeg != bEnd && *aBeg++ == *bBeg++)
        ++rv;
    return rv;
}

