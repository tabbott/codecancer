#pragma once

#include "CommandBase.hpp"

#include <string>
#include <vector>

class XmlReportCommand : public CommandBase {
public:
    void exec();

    std::string name() const {
        return "xml-report";
    }

    std::string description() const {
        return "generate a CPD compatible XML report";
    }

    void configureOptions();

    std::vector<std::string> const& requiredOptions() const;

private:
    std::string _indexDirPath;
    std::string _scanPath;
    std::string _outputPath;
};
