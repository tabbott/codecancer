#include "CodeCancer.hpp"

#include "IndexCommand.hpp"
#include "ScanCommand.hpp"
#include "BasicReportCommand.hpp"

#include <boost/format.hpp>

#include <sstream>
#include <stdexcept>

using boost::format;

namespace {
    const std::map<std::string, CommandBase::ptr> COMMANDS{
        {"index", CommandBase::ptr(new IndexCommand)},
        {"scan", CommandBase::ptr(new ScanCommand)},
        {"basic-report", CommandBase::ptr(new BasicReportCommand)}
        };
}

CodeCancer::CodeCancer() {
}

void CodeCancer::run(int argc, char** argv) {
    if (argc == 1) {
        std::stringstream ss;
        ss << "Available subcommands:\n\n";
        for (auto const& i : COMMANDS) {
            ss << "\t" << i.second->name() << " - " << i.second->description() << "\n";
        }

        throw std::runtime_error(ss.str());
    }

    auto iter = COMMANDS.find(argv[1]);
    if (iter == COMMANDS.end()) {
        throw std::runtime_error(str(format("Unknown subcommand %1%"
            ) % argv[1]));
    }

    iter->second->parseCommandLine(argc - 1, argv + 1);
    iter->second->exec();
}
