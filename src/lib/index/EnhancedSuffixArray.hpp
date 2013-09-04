#pragma once

#include "io/PodArray.hpp"
#include "SourceIndex.hpp"

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include <cassert>
#include <memory>
#include <string>

namespace {
    const std::string DEFAULT_SOURCE_FILENAME("source.txt");
    const std::string DEFAULT_POSITIONS_FILENAME("source.json");
    const std::string DEFAULT_SUFFIX_ARRAY_FILENAME("source.sa");
    const std::string DEFAULT_LCP_ARRAY_FILENAME("source.lcp");
}

struct EsaPaths {
    explicit EsaPaths(std::string const& dir)
        : _dir(dir)
    {
    }

    std::string subPath(std::string const& path) const {
        return (boost::filesystem::path(_dir) / path).string();
    }

    std::string sourcePath() const {
        return subPath(DEFAULT_SOURCE_FILENAME);
    }

    std::string positionsPath() const {
        return subPath(DEFAULT_POSITIONS_FILENAME);
    }

    std::string saPath() const {
        return subPath(DEFAULT_SUFFIX_ARRAY_FILENAME);
    }

    std::string lcpPath() const {
        return subPath(DEFAULT_LCP_ARRAY_FILENAME);
    }

private:
    std::string _dir;
};

template<typename T>
class EnhancedSuffixArray {
public:
    typedef T int_type;
    typedef MappedPodArray<int_type> ArrayType;

    explicit EnhancedSuffixArray(std::string const& dir)
        : _paths(dir)
    {
        namespace bio = boost::iostreams;

        _sourceBytes = ByteSource::create(
            new bio::mapped_file_source(_paths.sourcePath()));

        std::ifstream posStream(_paths.positionsPath());
        _sourceIndex.reset(new SourceIndex(_sourceBytes, posStream));

        _suffixArray.reset(new ArrayType(_paths.saPath()));
        _lcpArray.reset(new ArrayType(_paths.lcpPath()));

        assert(_sourceBytes->size() == _suffixArray->size());
        assert(_sourceBytes->size() == _lcpArray->size());
    }

    size_t size() const {
        return _suffixArray->size();
    }

    ArrayType const& suffixArray() const {
        return *_suffixArray;
    }

    ArrayType const& lcpArray() const {
        return *_lcpArray;
    }

    SourceIndex const& sourceIndex() const {
        return *_sourceIndex;
    }

private:
    EsaPaths _paths;
    ByteSource::ptr _sourceBytes;
    std::unique_ptr<SourceIndex> _sourceIndex;
    std::unique_ptr<ArrayType> _suffixArray;
    std::unique_ptr<ArrayType> _lcpArray;
};
