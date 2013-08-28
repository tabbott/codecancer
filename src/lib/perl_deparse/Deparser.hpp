#pragma once

#include <string>
#include <stdexcept>

// This is a class mainly to allow the constructor to verify things like
// "hey, we have perl, and it works".
class Deparser {
public:
    Deparser();

    void deparse(std::string const& in_path, std::string const& out_path);
};
