#pragma once

#include "SourceIndex.hpp"
#include "SuffixArray.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct DuplicateRegion {
    size_t begin1;
    size_t begin2;
    size_t length;
};

template<typename IntType>
class CloneHunter {
public:
    typedef IntType int_type;
    typedef SuffixArray<int_type> SAType;

    CloneHunter(SourceIndex const& idx, size_t minCloneLength)
        : _idx(idx)
        , _sa(idx.string())
        , _minCloneLength(minCloneLength)
    {
    }

    void goHunting() {
        for (size_t i = 1; i < _idx.size(); ++i) {
            size_t x = _sa[i-1];
            size_t y = _sa[i];

            size_t len = _idx.longestCommonPrefix(x, y);
            if (len > _minCloneLength) {
                collect(x, y, len);
            }
        }
    }

    void debugPrintDuplicates() {
        for (auto const& dup : _duplicates) {
            size_t x = dup.begin1;
            size_t y = dup.begin2;
            size_t len = dup.length;

            SourceFile filex = _idx.fileAtOffset(x);
            SourceFile filey = _idx.fileAtOffset(y);

            std::cout << "----------\n"
                << filex.name << ":" << filex.globalToFileOffset(x)
                << " and " << filey.name << ":" << filey.globalToFileOffset(y)
                << "\n";
            std::cout << "\t" << _idx.string().substr(x, len) << "\n";
        }
    }

private:
    void collect(size_t x, size_t y, size_t len) {
        DuplicateRegion dup{x, y, len};
        // FIXME: boost ICL to dedup subintervals!
        _duplicates.push_back(std::move(dup));
    }

private:
    SourceIndex const& _idx;
    SAType _sa;
    size_t _minCloneLength;
    std::vector<DuplicateRegion> _duplicates;
};
