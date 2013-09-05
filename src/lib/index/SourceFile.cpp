#include "SourceFile.hpp"

#include <json/reader.h>
#include <boost/format.hpp>

using boost::format;

std::vector<SourceFile> SourceFile::fromJson(std::istream& in) {
    std::vector<SourceFile> rv;

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(in, root)) {
        throw std::runtime_error(str(format("Failed to parse json file: %1%")
            % reader.getFormattedErrorMessages()));
    }

    Json::Value const& fileNames(root["filenames"]);
    Json::Value const& fileStarts(root["file_start_positions"]);
    Json::Value const& fileSizes(root["file_sizes"]);
    Json::Value const& allLineStarts(root["line_start_positions"]);

    // FIXME: check fileSizes too
    if (fileNames.size() != fileStarts.size() || fileNames.size() != fileSizes.size()) {
        throw std::runtime_error(str(format(
            "While parsing input json: mismatch between "
            "filenames, file_start_positions, and file_sizes arrays:"
            " (%1% vs %2% vs %3%)"
            ) % fileNames.size() % fileStarts.size() % fileSizes.size()));
    }

    for (Json::ArrayIndex i = 0; i < fileNames.size(); ++i) {
        auto const& name = fileNames[i].asString();
        auto const& startPos = fileStarts[i].asUInt64();
        size_t lastPos = startPos + fileSizes[i].asUInt64() - 1;
        auto const& lineStarts = allLineStarts[i];

        rv.emplace_back(SourceFile{name, startPos, lastPos});
        for (Json::ArrayIndex j = 0; j < lineStarts.size(); ++j) {
            rv.back().lineStartPositions.push_back(lineStarts[j].asUInt64());
        }
    }

    return rv;
}

size_t SourceFile::globalToFileOffset(size_t offset) const {
    return offset - firstByte;
}

bool SourceFile::operator==(SourceFile const& rhs) const {
    return name == rhs.name
        && firstByte == rhs.firstByte
        && lastByte == rhs.lastByte
        && lineStartPositions == rhs.lineStartPositions
        ;
}

size_t SourceFile::size() const {
    return lastByte - firstByte + 1;
}

std::ostream& operator<<(std::ostream& s, SourceFile const& sf) {
    s << "name: '" << sf.name << "', firstByte: " << sf.firstByte
        << ", lastByte: " << sf.lastByte << ", lines: [";

    bool first = true;
    for (auto const& l : sf.lineStartPositions) {
        if (!first)
            s << ", ";
        s << l;
        first = false;
    }
    s << "]";
    return s;
}
