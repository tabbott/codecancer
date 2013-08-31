#include "index/MaximalIntervalFilter.hpp"
#include "index/LcpArray.hpp"
#include "index/SourceFileList.hpp"
#include "index/SourceIndex.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace std;

template<typename T, typename SuffixArray>
struct Reporter {
    Reporter(std::ostream& s, SuffixArray const& sa)
        : _s(s)
        , _sa(sa)
    {
    }

    void operator()(LcpInterval const& x) const {
        _s << x.lcp << ": ";
        for (size_t i = x.leftBound; i <= x.rightBound; ++i) {
            _s << _sa[i];
            if (i < x.rightBound) {
                _s << ", ";
            }
        }
        _s << "\n";
    }

    std::ostream& _s;
    SuffixArray const& _sa;
};

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Give source file list and min region size!\n";
        return 1;
    }

    size_t minRegion = atoi(argv[2]);

    SourceFileList files = SourceFileList::fromFile(argv[1]);
    cout << "Found " << files.nonEmptyFiles().size() << " files, "
        << files.totalSourceBytes() << " bytes.\n";

    SourceIndex sidx;
    sidx.addSources(files);

    auto sa = makeSuffixArray<uint32_t>(sidx.string());
    vector<uint32_t> lcp;
    lcp.reserve(sa.size());
    makeLcpArray(sidx, sa, back_inserter(lcp));

    Reporter<uint32_t, vector<uint32_t>> clark(cout, sa);
    std::function<void(LcpInterval const&)> cb(clark);
    MaximalIntervalFilter<vector<uint32_t>> dude(minRegion, sa, sidx.string(), cb);
    visitLcpIntervals(lcp, dude);

    return 0;
}
