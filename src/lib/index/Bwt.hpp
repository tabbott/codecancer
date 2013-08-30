#pragma once

#include <cassert>
#include <string>

template<typename SaType>
size_t bwtFromSa(std::string const& text, SaType const& sa, std::string& bwt) {
    assert(text.size() == sa.size());
    size_t primaryIndex(0u);

    bwt.resize(text.size());

    bwt[0] = *text.rbegin();
    size_t i;
    for (i = 0; sa[i] != 0; ++i) {
        bwt[i + 1] = text[sa[i] - 1];
    }

    primaryIndex = i + 1;

    for(++i; i < text.size(); ++i) {
        bwt[i] = text[sa[i] - 1];
    }

    return primaryIndex;
}

