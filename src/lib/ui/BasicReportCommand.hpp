#include "CommandBase.hpp"

#include <string>
#include <vector>

class BasicReportCommand : public CommandBase {
public:
    void exec();

    std::string name() const { return "basic-report"; }
    std::string description() const { return "Show some stuff"; }

    void configureOptions();

    std::vector<std::string> const& requiredOptions() const;

private:
    std::string _indexDir;
    std::string _scanPath;
    std::string _outputPath;
};
