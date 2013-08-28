#include "Deparser.hpp"

#include <cstdlib>
#include <sstream>

Deparser::Deparser() {
}

void Deparser::deparse(std::string const& input_path, std::string const& output_path) {
    std::stringstream cmd;
    cmd << "perl -MO=Deparse,-si0 " << input_path << " > " << output_path;
    int rv = system(cmd.str().c_str());
    if (rv == -1) {
        throw std::runtime_error("system() failed in a bad way while deparsing");
    }

    if (rv != 0) {
        // FIXME: proper exception type
        throw std::runtime_error("Deparse failed: " + cmd.str());
    }
}
