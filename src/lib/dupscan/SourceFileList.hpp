#pragma once

#include <fstream>
#include <string>
#include <vector>

class SourceFileList {
public:
    static SourceFileList fromFile(std::string const& path);

    SourceFileList();

    std::vector<std::string> const& nonEmptyFiles() const;
    std::vector<std::string> const& emptyFiles() const;

    size_t totalSourceBytes() const;

private:
    size_t _totalSourceBytes;
    std::vector<std::string> _emptyFiles;
    std::vector<std::string> _nonEmptyFiles;
};

