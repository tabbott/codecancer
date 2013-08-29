#pragma once

#include <cstddef>

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
