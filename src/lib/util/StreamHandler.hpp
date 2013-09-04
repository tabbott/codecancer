#pragma once

#include <boost/format.hpp>

#include <fstream>
#include <iostream>
#include <unordered_map>

namespace {
}


class StreamHandler {
public:
    ~StreamHandler() {
        for (auto i : _streams) {
            delete i.second;
        }
    }

    template<typename StreamType>
    std::iostream* open(std::string const& path, bool allowDuplicate = false) {
        using boost::format;

        auto iter = _streams.find(path);
        if (iter != _streams.end()) {
            if (allowDuplicate)
                return iter->second;

            throw std::runtime_error(str(format(
                "Attempted to open file %1% twice") % path));
        }

        std::unique_ptr<std::iostream> f(openFile<StreamType>(path));
        if (!*f) {
            throw std::runtime_error(str(format(
                "Failed to open file %1%") % path));
        }

        std::iostream* fptr = f.get();
        _streams[path] = fptr;
        f.release();
        return fptr;
    }

private:
    template<typename T>
    std::unique_ptr<std::iostream> openFile(std::string const& path);

private:
    std::unordered_map<std::string, std::iostream*> _streams;
};

template<>
inline
std::unique_ptr<std::iostream> StreamHandler::openFile<std::istream>(std::string const& path) {
    return std::unique_ptr<std::iostream>(new std::fstream(path, std::ios::in));
}

template<>
inline
std::unique_ptr<std::iostream> StreamHandler::openFile<std::ostream>(std::string const& path) {
    return std::unique_ptr<std::iostream>(new std::fstream(path, std::ios::out));
}


