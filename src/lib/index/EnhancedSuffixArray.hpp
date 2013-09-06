#pragma once

#include "LcpArray.hpp"
#include "SourceIndex.hpp"
#include "SuffixArray.hpp"
#include "io/PodArray.hpp"
#include "io/StreamHandler.hpp"

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include <iterator>
#include <cassert>
#include <memory>
#include <string>

namespace {
    const std::string DEFAULT_SOURCE_FILENAME("source.txt");
    const std::string DEFAULT_POSITIONS_FILENAME("source.json");
    const std::string DEFAULT_SUFFIX_ARRAY_FILENAME("source.sa");
    const std::string DEFAULT_LCP_ARRAY_FILENAME("source.lcp");
}

class EsaPaths {
public:
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

    static std::shared_ptr<EnhancedSuffixArray> build(
        SourceIndex const& sidx, std::string const& dir
        )
    {
        { // This block makes sure streams is out of scope before return
            StreamHandler streams;
            EsaPaths paths(dir);
            auto srcStream = streams.open<std::ostream>(paths.sourcePath());
            auto posStream = streams.open<std::ostream>(paths.positionsPath());
            auto saStream = streams.open<std::ostream>(paths.saPath());
            auto lcpStream = streams.open<std::ostream>(paths.lcpPath());
            sidx.write(*srcStream, *posStream);

            auto suffixArray = makeSuffixArray<T>(sidx.data(), sidx.size());
            std::vector<T> lcpArray;
            lcpArray.reserve(suffixArray.size());
            makeLcpArray(sidx, suffixArray, std::back_inserter(lcpArray));
            writePodArray(*saStream, suffixArray.begin(), suffixArray.end());
            writePodArray(*lcpStream, lcpArray.begin(), lcpArray.end());
        }

        return std::shared_ptr<EnhancedSuffixArray>(new EnhancedSuffixArray(dir));
    }


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

    // This is mainly for use in test cases/debugging. If you try to print a
    // large suffix array, then your terminal is probably going to catch on
    // fire.
    friend std::ostream& operator<<(std::ostream& s, EnhancedSuffixArray const& esa) {
        auto const& sa = esa.suffixArray();
        auto const& lcp = esa.lcpArray();
        auto const& sidx = esa.sourceIndex();

        s << "#Idx\tSA\tLCP\tBWT\tSuffix\n";
        for (size_t i = 0; i < sa.size(); ++i) {
            s << i << "\t" << sa[i] << "\t" << lcp[i] << "\t";
            if (sa[i] != 0) {
                char c = sidx.data()[sa[i] - 1];
                if (c == '\n')
                    c = '+';
                s << c;
            }
            s << "\t[";
            for (size_t j = sa[i]; j < sa.size(); ++j) {
                char c = sidx.data()[j];
                if (c == '\n')
                    c = '+';
                s << c;
            }
            s << "]\n";
        }
        return s;
    }

private:
    EsaPaths _paths;
    ByteSource::ptr _sourceBytes;
    std::unique_ptr<SourceIndex> _sourceIndex;
    std::unique_ptr<ArrayType> _suffixArray;
    std::unique_ptr<ArrayType> _lcpArray;
};
