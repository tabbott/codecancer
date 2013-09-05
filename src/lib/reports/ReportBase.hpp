#pragma once

class LcpInterval;

class ReportBase {
public:
    virtual ~ReportBase() {}

    virtual void observe(LcpInterval const& interval) = 0;

    virtual void finalize() {}
};
