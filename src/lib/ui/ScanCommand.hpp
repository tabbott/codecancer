#include "CommandBase.hpp"

#include <string>
#include <vector>

class ScanCommand : public CommandBase {
public:
    void exec();

    std::string name() const { return "scan"; }
    std::string description() const { return "Scan an indexed source code bundle"; }

    void configureOptions();

    std::vector<std::string> const& requiredOptions() const;

private:
    std::string _indexDir;
    std::string _outputFile;
    size_t _minRegion;
};
