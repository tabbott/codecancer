#pragma once

#include <cassert>
#include <cstddef>
#include <istream>
#include <ostream>

struct LcpInterval {
    size_t lcp;
    size_t leftBound;
    size_t rightBound;

    static LcpInterval Null;

    size_t size() const;
    bool operator==(LcpInterval const& rhs) const;
    bool operator!=(LcpInterval const& rhs) const;
    void print(std::ostream& out) const;
};

std::ostream& operator<<(std::ostream& out, LcpInterval const& lcpi);
std::istream& operator>>(std::istream& in, LcpInterval& lcpi);
