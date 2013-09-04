#pragma once

#include "util/StreamHandler.hpp"

#include <boost/program_options.hpp>

#include <memory>
#include <string>
#include <vector>

class CommandBase {
public:
    typedef std::shared_ptr<CommandBase> ptr;

    CommandBase();
    virtual ~CommandBase() {}

    virtual void exec() = 0;
    virtual std::string name() const = 0;
    virtual std::string description() const = 0;
    virtual std::vector<std::string> const& requiredOptions() const = 0;

    virtual void configureOptions() = 0;
    virtual void parseCommandLine(int argc, char** argv);

    virtual bool hidden() const {
        return false;
    }


protected:
    StreamHandler _streams;
    boost::program_options::options_description _opts;
    boost::program_options::positional_options_description _posOpts;
};
