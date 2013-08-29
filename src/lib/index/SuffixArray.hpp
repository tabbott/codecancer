#pragma once

#include <divsufsort.h>
#include <divsufsort64.h>

#include <iostream>
#include <stdexcept>
#include <string>

// FIXME: pay attention to byte order in IO
namespace {
    void divsufsort_wrapper(std::string const& seq, uint64_t* data) {
        sauchar_t const* seqptr = reinterpret_cast<sauchar_t const*>(seq.data());
        saidx64_t* arr = reinterpret_cast<saidx64_t*>(data);
        if (divsufsort64(seqptr, arr, seq.size()) != 0) {
            throw std::runtime_error("Failed to build suffix array!");
        }
    }

    void divsufsort_wrapper(std::string const& seq, uint32_t* data) {
        sauchar_t const* seqptr = reinterpret_cast<sauchar_t const*>(seq.data());
        saidx_t* arr = reinterpret_cast<saidx_t*>(data);
        if (divsufsort(seqptr, arr, seq.size()) != 0) {
            throw std::runtime_error("Failed to build suffix array!");
        }
    }
}

template<typename T>
class SuffixArray {
public:
    typedef T value_type;

    SuffixArray() {}

    SuffixArray(value_type* data, size_t n)
        : _ownsMemory(false)
        , _size(n)
        , _sa(data)
    {
    }

    explicit SuffixArray(std::string const& sequence)
        : _ownsMemory(true)
        , _size(sequence.size())
        , _sa(new value_type[_size])
    {
        divsufsort_wrapper(sequence, _sa);
    }

    ~SuffixArray() {
        if (_ownsMemory)
            delete[] _sa;

        _sa = 0;
    }

    // for each index in the sa, call f(index, _sa[index])
    template<typename Function>
    void foreach(Function f) const {
        for (size_t i = 0; i < _size; ++i) {
            f(i, (*this)[i]);
        }
    }

    value_type operator[](size_t idx) const {
        return _sa[idx];
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    bool operator==(SuffixArray const& rhs) const {
        return _sa == rhs._sa;
    }

    friend std::ostream& operator<<(std::ostream& s, SuffixArray const& sa) {
        for (size_t i = 0; i < sa.size(); ++i) {
            s << i << ": " << sa[i] << "\n";
        }
        return s;
    }

    value_type const* data() const {
        return _sa.data();
    }

private:
    bool _ownsMemory;
    size_t _size;
    value_type* _sa;
};
