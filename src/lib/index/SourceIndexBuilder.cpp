#include "SourceIndexBuilder.hpp"

#include "SourceFile.hpp"
#include "SourceIndex.hpp"
#include "util/Sequence.hpp"

#include <boost/format.hpp>

#include <sstream>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>

using boost::format;

void SourceIndexBuilder::addSource(std::string const& name, std::istream& src) {
    src.unsetf(std::ios::skipws);
    SourceFile sf{name, _text.size()};
    std::copy(std::istream_iterator<char>(src), std::istream_iterator<char>(),
            std::back_inserter(_text));

    if (_text.size() == sf.firstByte) {
        throw std::runtime_error(str(format(
            "Empty source file: %1%") %name));
    }

    sf.lastByte = _text.size() - 1;
    // find the beginning of each line
    sf.lineStartPositions.push_back(0);
    size_t fileSize = sf.size();
    std::function<void(size_t)> collector = [&sf, fileSize] (size_t idx) {
        if (idx + 1 < fileSize)
            sf.lineStartPositions.push_back(idx + 1);
    };

    foreachIndexIf(
        _text.begin() + sf.firstByte,
        _text.begin() + sf.lastByte + 1,
        collector,
        [] (char c) { return c == '\n'; });

    _sourceFiles.push_back(std::move(sf));
}

std::shared_ptr<SourceIndex> SourceIndexBuilder::build() {
    ByteSource::ptr bs(ByteSource::create(new std::string(std::move(_text))));
    return std::shared_ptr<SourceIndex>(new SourceIndex(bs, _sourceFiles));
}
