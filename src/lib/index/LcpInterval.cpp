#include "LcpArray.hpp"

LcpInterval LcpInterval::Null{0, 0, 0};


size_t LcpInterval::size() const {
    assert(rightBound >= leftBound);
    return rightBound - leftBound + 1;
}

bool LcpInterval::operator==(LcpInterval const& rhs) const {
    return lcp == rhs.lcp
        && leftBound == rhs.leftBound
        && rightBound == rhs.rightBound
        ;
}

bool LcpInterval::operator!=(LcpInterval const& rhs) const {
    return !(*this == rhs);
}

void LcpInterval::print(std::ostream& out) const {
    out << "{lcp = " << lcp << ", interval = ["
        << leftBound << ", " << rightBound << "]}";
}

std::ostream& operator<<(std::ostream& out, LcpInterval const& lcpi) {
    out << lcpi.lcp << "\t" << lcpi.leftBound << "\t" << lcpi.rightBound;
    return out;
}

std::istream& operator>>(std::istream& in, LcpInterval& lcpi) {
    in >> lcpi.lcp >> lcpi.leftBound >> lcpi.rightBound;
    return in;
}


