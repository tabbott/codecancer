#pragma once

#include <boost/format.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include <iterator>
#include <ostream>

template<typename T>
T identityTransform(T const& value) {
    return value;
}

template<typename T, typename TransformType>
void writePodArray(
        std::ostream& out,
        T const* x,
        size_t n,
        TransformType transform = &identityTransform<T>)
{
    // FIXME: endianness. Pass endian converter as a TransformType
    size_t eltSize = sizeof(T);
    // Write size of element
    out.write(reinterpret_cast<char const*>(&eltSize), sizeof(eltSize));
    // Write number of elements
    out.write(reinterpret_cast<char const*>(&n), sizeof(n));

    // Write elements
    for (size_t i = 0; i < n; ++i) {
        T value = transform(x[i]);
        out.write(reinterpret_cast<char const*>(&value), sizeof(value));
    }
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

        _eltSize = *this->convertRawPtr<size_t>(0);
        _size = *this->convertRawPtr<size_t>(sizeof(_eltSize));
        _data = this->convertRawPtr<T>(sizeof(_size) + sizeof(_eltSize));
    }



private:
    std::unique_ptr<boost::iostreams::mapped_file_source> _file;
    size_t _eltSize;
    size_t _size;
    char const* _rawData;
    size_t _rawSize;
    T const* _data;
};
