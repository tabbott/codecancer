#pragma once

#include <vector>
#include <string>

class LcpInterval;

class ReportBase {
public:
    virtual ~ReportBase() {}

    virtual void observe(LcpInterval const& interval) = 0;

    virtual void parseArgs(std::vector<std::string> const& args);

    virtual void finalize() {}
};
