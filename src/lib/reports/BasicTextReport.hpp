#pragma once

#include "ReportBase.hpp"
#include "metrics/DuplicateByteCounter.hpp"

#include <ostream>
#include <vector>

class EsaView;
class LcpInterval;

class BasicTextReport : public ReportBase {
public:
    BasicTextReport(EsaView const& esaView, std::ostream& out);

    void observe(LcpInterval const& interval);
    void reportInterval(LcpInterval const& interval);

    void finalize();

private:
    EsaView const& _esaView;
    std::ostream& _out;
    std::vector<LcpInterval> _intervals;
    DuplicateByteCounter _dupCounter;
};
