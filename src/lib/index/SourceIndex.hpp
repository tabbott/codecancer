#pragma once

#include "SourceFile.hpp"
#include "io/ByteSource.hpp"
#include "util/Sequence.hpp"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <istream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


class SourceIndex {
public:
    SourceIndex(ByteSource::ptr const& sourceText, std::vector<SourceFile> files);
    SourceIndex(ByteSource::ptr const& sourceText, std::istream& json);

    size_t longestCommonPrefix(size_t idx1, size_t idx2) const;
    SourceFile const& fileAtOffset(size_t offset) const;

    size_t size() const;
    bool empty() const;
    char const* data() const;

    std::vector<SourceFile> const& sourceFiles() const;

private:
    ByteSource::ptr _sourceText;
    std::vector<SourceFile> _sourceFiles;

    char const* _rawSource;
    size_t _rawSourceSize;
};

std::ostream& operator<<(std::ostream& s, SourceFile const& sf);
