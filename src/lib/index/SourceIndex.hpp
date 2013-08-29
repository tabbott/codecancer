#pragma once

#include "SourceFileList.hpp"
#include "prefix.hpp"

#include <boost/format.hpp>

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <istream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


struct SourceFile {
    std::string name;
    size_t firstByte;
    size_t lastByte;

    size_t globalToFileOffset(size_t offset) {
        return offset - firstByte;
    }
};

class SourceIndex {
public:
    SourceIndex& numFilesHint(size_t n) {
        _fileOffsets.reserve(n);
        return *this;
    }

    SourceIndex& numBytesHint(size_t n) {
        _sourceText.reserve(n);
        return *this;
    }

    void addSources(SourceFileList const& sources) {
        for (auto const& path : sources.nonEmptyFiles()) {
            std::ifstream in(path);
            if (!in) {
                throw std::runtime_error("Failed to open input file " + path);
            }
            addSource(path, in);
        }
    }

    void addSource(std::string const& path, std::istream& s) {
        s.unsetf(std::ios::skipws);
        SourceFile sf{path, _sourceText.size()};
        std::copy(std::istream_iterator<char>(s), std::istream_iterator<char>(),
                std::back_inserter(_sourceText));

        // FIXME: throw for 0 byte streams (firstByte == lastByte)
        if (_sourceText.size() == sf.firstByte) {
            throw std::runtime_error(boost::str(boost::format(
                "File %1% is empty!") % path));
        }

        sf.lastByte = _sourceText.size() - 1;

        _fileOffsets.push_back(std::move(sf));
    }

    size_t longestCommonPrefix(size_t idx1, size_t idx2) const {
        if (idx1 >= _sourceText.size() || idx2 >= _sourceText.size()) {
            throw std::range_error("out of bounds");
        }

        // FIXME: We could be smarter and avoid these log(n) lookups for
        // file offsets if we used \0 or something to denote file boundaries
        auto const& f1 = fileAtOffset(idx1);
        auto const& f2 = fileAtOffset(idx2);

        return commonPrefix(
            _sourceText.begin() + idx1, _sourceText.begin() + f1.lastByte + 1,
            _sourceText.begin() + idx2, _sourceText.begin() + f2.lastByte + 1);
    }

    SourceFile const& fileAtOffset(size_t offset) const {
        if (offset >= _sourceText.size()) {
            throw std::range_error(boost::str(boost::format(
                    "Index %1% out of bounds, source contains only %2% bytes"
                    ) % offset % _sourceText.size()));
        }

        auto iter = std::lower_bound(_fileOffsets.begin(), _fileOffsets.end(),
                offset, [] (SourceFile const& x, size_t off) {
                    return x.lastByte < off;
                });

        return *iter;
    }

    size_t size() const {
        return _sourceText.size();
    }

    bool empty() const {
        return _sourceText.empty();
    }

    std::string const& string() const {
        return _sourceText;
    }

private:
    std::vector<SourceFile> _fileOffsets;
    std::string _sourceText;
};
