#pragma once

#include <divsufsort.h>
#include <divsufsort64.h>

#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>

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

    template<typename T>
    std::vector<T> makeSuffixArray(char const* data, size_t size) {
        std::vector<T> sa(size);
        divsufsort_wrapper(data, sa.data());
        return sa;
    }

    template<typename T>
    std::vector<T> makeSuffixArray(std::string const& sequence) {
        return makeSuffixArray<T>(sequence.data(), sequence.size());
    }
}
