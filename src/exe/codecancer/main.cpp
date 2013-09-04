#include "index/MaximalIntervalFilter.hpp"
#include "index/LcpArray.hpp"
#include "index/SourceIndex.hpp"

#include <boost/iostreams/device/mapped_file.hpp>

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace std;
namespace bio = boost::iostreams;

template<typename T, typename SuffixArray>
struct Reporter {
    Reporter(std::ostream& s, SuffixArray const& sa)
        : _s(s)
        , _sa(sa)
    {
    }

    void operator()(LcpInterval const& x) const {
        _s << x.lcp << ":";
        for (size_t i = x.leftBound; i <= x.rightBound; ++i) {
            _s << _sa[i];
            if (i < x.rightBound) {
                _s << ",";
            }
        }
        _s << "\n";
    }

    std::ostream& _s;
    SuffixArray const& _sa;
};

int main(int argc, char** argv) {
    if (argc != 4) {
        cerr << "Give source file list, positions.json and min region size!\n";
        return 1;
    }

    ifstream json(argv[2]);
    size_t minRegion = atoi(argv[3]);

    ByteSource::ptr sourceBytes = ByteSource::create(new bio::mapped_file_source(argv[1]));
    SourceIndex sidx(sourceBytes, json);

    auto sa = makeSuffixArray<uint32_t>(sidx.data(), sidx.size());
    vector<uint32_t> lcp;
    lcp.reserve(sa.size());
    makeLcpArray(sidx, sa, back_inserter(lcp));

    Reporter<uint32_t, vector<uint32_t>> clark(cout, sa);
    std::function<void(LcpInterval const&)> cb(clark);
    MaximalIntervalFilter<vector<uint32_t>> dude(minRegion, sa, sidx.data(), cb);
    visitLcpIntervals(lcp, dude);

    return 0;
}
