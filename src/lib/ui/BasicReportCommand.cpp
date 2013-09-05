#include "BasicReportCommand.hpp"

#include "index/EnhancedSuffixArray.hpp"
#include "index/EsaView.hpp"
#include "index/LcpInterval.hpp"
#include "reports/BasicTextReport.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <vector>

namespace po = boost::program_options;

std::vector<std::string> const& BasicReportCommand::requiredOptions() const {
    static std::vector<std::string> req{"index-directory", "scan-file", "output-file"};
    return req;
}

void BasicReportCommand::configureOptions() {
    _opts.add_options()
        ("help,h", "this message")
        ("index-directory,i", po::value<std::string>(&_indexDir), "Source code index directory")
        ("scan-file,s", po::value<std::string>(&_scanPath), "scan result")
        ("output-file,o", po::value<std::string>(&_outputPath), "output file")
        ;

    _posOpts.add("index-directory", 1);
    _posOpts.add("scan-file", 1);
    _posOpts.add("output-file", 1);
}

void BasicReportCommand::exec() {
    std::iostream* scan = _streams.open<std::istream>(_scanPath);
    std::iostream* out = _streams.open<std::ostream>(_outputPath);
    EnhancedSuffixArray<uint32_t> esa(_indexDir);

    EsaView esaView(esa);
    BasicTextReport report(esaView, *out);

    LcpInterval interval;
    while (*scan >> interval) {
        report.observe(interval);
    }

    report.finalize();
}
