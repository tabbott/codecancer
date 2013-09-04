#include "IndexCommand.hpp"

#include "index/EnhancedSuffixArray.hpp"
#include "index/LcpArray.hpp"
#include "index/SuffixArray.hpp"
#include "io/ByteSource.hpp"
#include "io/PodArray.hpp"
#include "util/Timer.hpp"

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/program_options.hpp>

#include <fstream>

namespace bio = boost::iostreams;
namespace po = boost::program_options;

std::vector<std::string> const& IndexCommand::requiredOptions() const {
    static std::vector<std::string> req{"input-dir"};
    return req;
}

void IndexCommand::configureOptions() {
    _opts.add_options()
        ("help,h", "this message")
        ("input-dir,i", po::value<std::string>(&_bundlePath), "concatenated source code")
        ;

    _posOpts.add("input-dir", 1);
}

void IndexCommand::exec() {
    EsaPaths paths(_bundlePath);
    std::iostream* saStream = _streams.open<std::ostream>(paths.saPath());
    std::iostream* lcpStream = _streams.open<std::ostream>(paths.lcpPath());
    std::iostream* json = _streams.open<std::istream>(paths.positionsPath());

    ByteSource::ptr source(ByteSource::create(new bio::mapped_file_source(paths.sourcePath())));

    Timer<> timer;
    SourceIndex sidx(source, *json);
    std::cerr << "Loaded source data in " << timer.elapsed() << "\n\n";

    timer.reset();
    auto suffixArray = makeSuffixArray<uint32_t>(sidx.data(), sidx.size());
    std::cerr << "Created suffix array in " << timer.elapsed() << "\n";

    timer.reset();
    writePodArray(*saStream, suffixArray.begin(), suffixArray.end());
    std::cerr << "Wrote suffix array: " << paths.saPath()
        << ", " << timer.elapsed() << "\n\n";

    std::vector<uint32_t> lcpArray;
    timer.reset();
    lcpArray.reserve(suffixArray.size());
    makeLcpArray(sidx, suffixArray, std::back_inserter(lcpArray));
    std::cerr << "Created lcp array in " << timer.elapsed() << "\n";

    timer.reset();
    writePodArray(*lcpStream, lcpArray.begin(), lcpArray.end());
    std::cerr << "Wrote lcp array: " << paths.lcpPath()
        << ", " << timer.elapsed() << "\n\n";
}
