#pragma once

#include <cstddef>
#include <memory>

class DuplicateByteCounter {
public:
    DuplicateByteCounter();
    ~DuplicateByteCounter();

    void addInterval(size_t begin, size_t end);

    size_t value() const;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};
