#include "CommandBase.hpp"

#include <string>
#include <vector>

class IndexCommand : public CommandBase {
public:
    void exec();

    std::string name() const { return "index"; }
    std::string description() const { return "Index a source code bundle"; }

    void configureOptions();

    std::vector<std::string> const& requiredOptions() const;

private:
    std::string _bundlePath;
};
