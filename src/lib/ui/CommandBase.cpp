#include "CommandBase.hpp"

#include <boost/format.hpp>

#include <sstream>
#include <stdexcept>

namespace po = boost::program_options;
using boost::format;

CommandBase::CommandBase()
    : _opts("Available Options")
{
}

void CommandBase::parseCommandLine(int argc, char** argv) {
    configureOptions();

    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv)
            .options(_opts)
            .positional(_posOpts).run(),
        vm
    );
    po::notify(vm);

    if (vm.count("help")) {
        std::stringstream ss;
        ss << _opts;
        throw std::runtime_error(ss.str());
    }

    for (auto opt : requiredOptions()) {
        if (!vm.count(opt)) {
            throw std::runtime_error(str(format(
                "Required argument '%1%' missing"
                ) % opt));
        }
    }
}
