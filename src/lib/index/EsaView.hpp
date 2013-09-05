#pragma once

#include "EnhancedSuffixArray.hpp"
#include "util/UnsignedVectorView.hpp"

#include <cstddef>
#include <memory>

class EsaView {
private:
    typedef UnsignedVectorView ArrayType;

    struct Concept {
        virtual ~Concept() {}

        virtual size_t size() const = 0;
        virtual ArrayType const& suffixArray() const = 0;
        virtual ArrayType const& lcpArray() const = 0;
        virtual SourceIndex const& sourceIndex() const = 0;
    };

    template<typename T>
    struct Instance : Concept {
        explicit Instance(T const& obj)
            : _obj(obj)
            , _suffixArray(obj.suffixArray())
            , _lcpArray(obj.lcpArray())
        {
        }

        size_t size() const {
            return _obj.size();
        }

        ArrayType const& suffixArray() const {
            return _suffixArray;
        }

        ArrayType const& lcpArray() const {
            return _lcpArray;
        }

        SourceIndex const& sourceIndex() const {
            return _obj.sourceIndex();
        }

        T const& _obj;
        ArrayType _suffixArray;
        ArrayType _lcpArray;
    };

public:
    template<typename T>
    EsaView(T const& esa)
        : _obj(new Instance<T>(esa))
    {
    }

    size_t size() const {
        return _obj->size();
    }

    ArrayType const& suffixArray() const {
        return _obj->suffixArray();
    }

    ArrayType const& lcpArray() const {
        return _obj->lcpArray();
    }

    SourceIndex const& sourceIndex() const {
        return _obj->sourceIndex();
    }

private:
    std::unique_ptr<Concept> _obj;
};
