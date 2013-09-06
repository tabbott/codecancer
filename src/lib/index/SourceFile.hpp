#pragma once

#include <json/value.h>
#include <json/writer.h> // operator<<(ostream&, Json::Value)

#include <cstddef>
#include <istream>
#include <string>
#include <vector>

struct SourceFile {
    std::string name;
    size_t firstByte;
    size_t lastByte;
    std::vector<size_t> lineStartPositions;

    size_t globalToFileOffset(size_t offset) const;
    bool operator==(SourceFile const& rhs) const;

    size_t size() const;

    size_t lineNumberOfIndex(size_t index) const;

    static Json::Value toJson(std::vector<SourceFile> const& files);
    static std::vector<SourceFile> fromJson(std::istream& in);
};
