#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

class UnsignedVectorView {
private:
    struct Concept {
        virtual ~Concept() {}
        virtual uint64_t operator[](size_t idx) const = 0;
        virtual size_t size() const = 0;
    };

    template<typename T>
    struct Instance : public Concept {
        Instance(T const& obj)
            : _obj(obj)
        {
        }

        uint64_t operator[](size_t idx) const {
            return uint64_t{_obj[idx]};
        }

        size_t size() const {
            return _obj.size();
        }

        T const& _obj;
    };

public:
    template<typename T>
    UnsignedVectorView(T const& vec)
        : _obj(new Instance<T>(vec))
    {
    }

    size_t size() const {
        return _obj->size();
    }

    uint64_t operator[](size_t idx) const {
        return (*_obj)[idx];
    }

private:
    std::unique_ptr<Concept> _obj;
};
