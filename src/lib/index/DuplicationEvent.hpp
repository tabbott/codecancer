#pragma once

#include <cassert>
#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

struct DuplicationEvent {
    struct Region {
        std::string path;
        size_t firstLine;
        size_t lastLine;

        size_t lineCount() const {
            assert(lastLine >= firstLine);
            return lastLine - firstLine + 1;
        }

        bool operator==(Region const& rhs) const {
            return path == rhs.path
                && firstLine == rhs.firstLine
                && lastLine == rhs.lastLine
                ;
        }
    };

    size_t size;
    std::vector<Region> regions;

    bool operator==(DuplicationEvent const& rhs) const {
        return size == rhs.size && regions == rhs.regions;
    }

    friend std::ostream& operator<<(std::ostream& s, DuplicationEvent const& ev) {
        s << "Duplication of " << ev.size << " bytes:\n";
        for (auto const& r : ev.regions) {
            s << "\t" << r.path << " lines " << r.firstLine << "-"
                << r.lastLine << "\n";
        }
        return s;
    }
};
