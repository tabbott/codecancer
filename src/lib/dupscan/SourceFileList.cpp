#include "SourceFileList.hpp"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

using boost::format;
namespace bfs = boost::filesystem;

SourceFileList::SourceFileList()
    : _totalSourceBytes(0)
{
}

SourceFileList SourceFileList::fromFile(std::string const& path) {
    SourceFileList rv;
    std::ifstream in(path);
    std::string line;
    if (!in) {
        throw std::runtime_error(str(format(
                "Failed to open input source list file %1%"
                ) % path));
    }
    while (std::getline(in, line)) {
        size_t size = bfs::file_size(line);
        rv._totalSourceBytes += size;

        if (size == 0u)
            rv._emptyFiles.push_back(line);
        else
            rv._nonEmptyFiles.push_back(line);
    }
    return rv;
}

std::vector<std::string> const& SourceFileList::nonEmptyFiles() const {
    return _nonEmptyFiles;
}

std::vector<std::string> const& SourceFileList::emptyFiles() const {
    return _emptyFiles;
}

size_t SourceFileList::totalSourceBytes() const {
    return _totalSourceBytes;
}
