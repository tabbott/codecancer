#include "SourceIndex.hpp"

#include <boost/format.hpp>

#include <stdexcept>
#include <utility>

using boost::format;

SourceIndex::SourceIndex(ByteSource::ptr const& sourceText, std::vector<SourceFile> files)
    : _sourceText(sourceText)
    , _sourceFiles(std::move(files))
    , _rawSource(sourceText->data())
    , _rawSourceSize(sourceText->size())
{
}

SourceIndex::SourceIndex(ByteSource::ptr const& sourceText, std::istream& json)
    : _sourceText(sourceText)
    , _sourceFiles(SourceFile::fromJson(json))
    , _rawSource(sourceText->data())
    , _rawSourceSize(sourceText->size())
{
}

size_t SourceIndex::longestCommonPrefix(size_t idx1, size_t idx2) const {
    if (idx1 >= _sourceText->size() || idx2 >= _sourceText->size()) {
        throw std::range_error("out of bounds");
    }

    // FIXME: We could be smarter and avoid these log(n) lookups for
    // file offsets if we used \0 or something to denote file boundaries
    auto const& f1 = fileAtOffset(idx1);
    auto const& f2 = fileAtOffset(idx2);

    return commonPrefix(
        _rawSource + idx1, _rawSource + f1.lastByte + 1,
        _rawSource + idx2, _rawSource + f2.lastByte + 1);
}

SourceFile const& SourceIndex::fileAtOffset(size_t offset) const {
    if (offset >= _rawSourceSize) {
        throw std::range_error(str(format(
                "Index %1% out of bounds, source contains only %2% bytes"
                ) % offset % _rawSourceSize));
    }

    auto iter = std::lower_bound(_sourceFiles.begin(), _sourceFiles.end(),
            offset, [] (SourceFile const& x, size_t off) {
                return x.lastByte < off;
            });

    return *iter;
}

size_t SourceIndex::size() const {
    return _rawSourceSize;
}

bool SourceIndex::empty() const {
    return size() == 0u;
}

char const* SourceIndex::data() const {
    return _rawSource;
}

std::vector<SourceFile> const& SourceIndex::sourceFiles() const {
    return _sourceFiles;
}
