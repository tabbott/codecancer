#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>

class ByteSource {
private:
    struct Concept {
        virtual ~Concept() {}
        virtual char const* data() const = 0;
        virtual size_t size() const = 0;
    };

    template<typename T>
    struct Instance : Concept {
        Instance(T const* obj)
            : _obj(obj)
        {
        }

        char const* data() const {
            return _obj->data();
        }

        size_t size() const {
            return _obj->size();
        }

        std::unique_ptr<T const> _obj;
    };

    template<class T>
    ByteSource(T const* obj)
        : _obj(new Instance<T>(obj))
    {
    }

public:
    ByteSource(ByteSource const&) = delete;
    ByteSource& operator=(ByteSource const&) = delete;

    typedef std::shared_ptr<ByteSource> ptr;
    typedef char const* const_iterator;

    template<typename T>
    static ptr create(T const* obj) {
        return ptr(new ByteSource(obj));
    }

    char const* data() const {
        return _obj->data();
    }

    size_t size() const {
        return _obj->size();
    }

    const_iterator begin() const {
        return data();
    }

    const_iterator end() const {
        return data() + size();
    }

private:
    std::unique_ptr<Concept> _obj;
};
