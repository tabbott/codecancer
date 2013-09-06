#include "ReportBase.hpp"

#include <sstream>
#include <boost/format.hpp>

using boost::format;

void ReportBase::parseArgs(std::vector<std::string> const& args) {
    if (!args.empty()) {
        std::stringstream ss;
        for (auto const& arg : args) {
            ss << "\t" << arg << "\n";
        }

        throw std::runtime_error(str(format("Unexpected report arguments:\n%1%")
            % ss.str()));
    }
}
