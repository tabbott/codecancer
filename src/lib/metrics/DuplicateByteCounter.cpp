#include "DuplicateByteCounter.hpp"

#include <boost/icl/interval_set.hpp>
#include <boost/icl/discrete_interval.hpp>

#include <iostream>

namespace bi = boost::icl;

class DuplicateByteCounter::Impl {
public:
    typedef bi::interval_set<size_t> SetType;

    void addInterval(size_t begin, size_t end) {
        _intervals.insert(bi::discrete_interval<size_t>(begin, end));
    }

    size_t value() const {
        return bi::length(_intervals);
    }

private:
    SetType _intervals;
};

DuplicateByteCounter::DuplicateByteCounter()
    : _impl(new Impl)
{
}

DuplicateByteCounter::~DuplicateByteCounter() {
}

void DuplicateByteCounter::addInterval(size_t begin, size_t end) {
    _impl->addInterval(begin, end);
}

size_t DuplicateByteCounter::value() const {
    return _impl->value();
}
