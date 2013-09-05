#include "BasicTextReport.hpp"

#include "index/EsaView.hpp"
#include "index/SourceIndex.hpp"
#include "index/LcpInterval.hpp"

#include <algorithm>
#include <functional>

BasicTextReport::BasicTextReport(EsaView const& esaView, std::ostream& out)
    : _esaView(esaView)
    , _out(out)
{
}

void BasicTextReport::observe(LcpInterval const& interval) {
    _intervals.push_back(interval);
}

void BasicTextReport::reportInterval(LcpInterval const& interval) {
    auto const& sidx = _esaView.sourceIndex();
    auto const& sa = _esaView.suffixArray();
    _out << "Duplication of " << interval.lcp << " bytes:\n";

    for (size_t i = interval.leftBound; i <= interval.rightBound; ++i) {
        size_t offset = sa[i];
        _dupCounter.addInterval(offset, offset + interval.lcp);
        SourceFile const& sourceFile = sidx.fileAtOffset(offset);
        _out << "\t" << sourceFile.name << ":"
            << sourceFile.globalToFileOffset(offset) << "\n";
    }
}

void BasicTextReport::finalize() {
    std::sort(_intervals.begin(), _intervals.end(),
        [] (LcpInterval const& x, LcpInterval const& y) {
            return x.lcp > y.lcp;
        });

    using std::placeholders::_1;
    std::for_each(_intervals.begin(), _intervals.end(),
        std::bind(&BasicTextReport::reportInterval, this, _1));

    size_t dupBytes = _dupCounter.value();
    size_t totalBytes = _esaView.sourceIndex().size();

    _out << "Total duplicated bytes: " << dupBytes << "\n";
    _out << "Effective source bytes: " << totalBytes << "\n";
    _out << "Duplicated source percentage: "
        << (100.0 * dupBytes) / totalBytes << "%\n";

}
