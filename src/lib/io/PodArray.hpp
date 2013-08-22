#pragma once

#include <boost/format.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include <cstddef>
#include <iterator>
#include <ostream>
#include <stdexcept>

template<typename IteratorType>
void writePodArray(
        std::ostream& out,
        IteratorType begin,
        IteratorType end)
{
    auto elements = size_t{0};

    // Write elements
    while (begin != end) {
        out.write(reinterpret_cast<char const*>(&*begin), sizeof(*begin));
        ++begin;
        ++elements;
    }

    size_t eltSize = sizeof(
            typename std::iterator_traits<IteratorType>::value_type);
    // Write size of element
    out.write(reinterpret_cast<char const*>(&eltSize), sizeof(eltSize));
    // Write number of elements
    out.write(reinterpret_cast<char const*>(&elements), sizeof(elements));
}

template<typename T>
struct MappedPodArray {
public:
    MappedPodArray(std::string const& path)
        : _file(new boost::iostreams::mapped_file_source(path))
        , _rawData(_file->data())
        , _rawSize(_file->size())
    {
        init();
    }

    MappedPodArray(char const* data, size_t rawSize)
        : _rawData(data)
        , _rawSize(rawSize)
    {
        init();
    }

    template<typename S>
    S const* convertRawPtr(size_t idx) const {
        return reinterpret_cast<S const*>(_rawData + idx);
    }

    size_t size() const {
        return _size;
    }

    bool empty() const {
        return size() == 0;
    }

    T const* data() const {
        return _data;
    }

    T const& operator[](size_t idx) const {
        return _data[idx];
    }

private:
    void init() {
        using boost::format;

        if (_rawSize < sizeof(_eltSize) + sizeof(_size)) {
            throw std::runtime_error(str(format(
                "File %1% is not a proper PodArray")));
        }

        _eltSize = *this->convertRawPtr<size_t>(
                _rawSize - sizeof(_size) - sizeof(_eltSize));
        if (_eltSize != sizeof(T)) {
            throw std::runtime_error(str(format(
                "Element size mismatch: expected %1%, observed %2%"
                ) % sizeof(T) % _eltSize));
        }

        _size = *this->convertRawPtr<size_t>(_rawSize - sizeof(_size));
        _data = this->convertRawPtr<T>(0);
    }



private:
    std::unique_ptr<boost::iostreams::mapped_file_source> _file;
    size_t _eltSize;
    size_t _size;
    char const* _rawData;
    size_t _rawSize;
    T const* _data;
};
