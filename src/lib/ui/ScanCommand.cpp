#include "ScanCommand.hpp"

#include "index/LcpArray.hpp"
#include "index/EnhancedSuffixArray.hpp"
#include "index/MaximalIntervalFilter.hpp"
#include "util/Timer.hpp"

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <vector>

namespace po = boost::program_options;

std::vector<std::string> const& ScanCommand::requiredOptions() const {
    static std::vector<std::string> req{"index-directory", "output-file", "min-region"};
    return req;
}

void ScanCommand::configureOptions() {
    _minRegion = 300;
    _opts.add_options()
        ("help,h", "this message")
        ("index-directory,i", po::value<std::string>(&_indexDir), "Source code index directory")
        ("output-file,o", po::value<std::string>(&_outputFile), "output file")
        ("min-region,m", po::value<size_t>(&_minRegion), "min region (bytes)")
        ;

    _posOpts.add("index-directory", 1);
    _posOpts.add("output-file", 1);
    _posOpts.add("min-region", 1);
}

void ScanCommand::exec() {
    std::iostream* out = _streams.open<std::ostream>(_outputFile);
    EnhancedSuffixArray<uint32_t> esa(_indexDir);

    size_t counter(0);
    std::function<void(LcpInterval const&)> cb = [&out, &counter] (LcpInterval const& x) {
        *out << x << "\n";
        ++counter;
    };

    std::function<void(LcpInterval const&)> filter(
        MaximalIntervalFilter<EnhancedSuffixArray<uint32_t>::ArrayType>(
            _minRegion, esa.suffixArray(), esa.sourceIndex(), cb)
        );

    Timer<> timer;
    visitLcpIntervals(esa.lcpArray(), filter);
    std::cerr << "Observed " << counter << " intervals in "
        << timer.elapsed() << "\n";
}
