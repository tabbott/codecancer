#pragma once

#include <boost/chrono/chrono_io.hpp>

#include <cstddef>
#include <ostream>
#include <string>
#include <sstream>

template<
        typename _Clock = boost::chrono::high_resolution_clock,
        typename _DefaultUnits = boost::chrono::milliseconds
        >
class Timer {
public:
    typedef _Clock clock_type;
    typedef _DefaultUnits default_units;

    Timer() : _start(clock_type::now()) {}

    void reset() {
        _start = clock_type::now();
    }

    template<typename T>
    T elapsed_as() const {
        return boost::chrono::duration_cast<T>(clock_type::now() - _start);
    }

    default_units elapsed() const {
        return elapsed_as<default_units>();
    }

private:
    typename clock_type::time_point _start;
};
