#include "index/DuplicationEventFactory.hpp"
#include "index/DuplicationEvent.hpp"

#include "index/EnhancedSuffixArray.hpp"
#include "index/LcpArray.hpp"
#include "index/MaximalIntervalFilter.hpp"
#include "index/SourceIndex.hpp"
#include "index/SourceIndexBuilder.hpp"
#include "util/TempFile.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class TestDuplicationEventFactory : public ::testing::Test{
public:
    typedef DuplicationEventFactory<EnhancedSuffixArray<uint32_t>> Factory;
    typedef EnhancedSuffixArray<uint32_t>::ArrayType ArrayType;

    void SetUp() {
        src1 << "\n\nabc\n\ndef\n\n";
        src2 << "\n\nabc\n\ndefg\n\n";
        src3 << "\n\nabc\n\ndefgh\n\n";

        builder.addSource("src/1", src1);
        builder.addSource("src/2", src2);
        builder.addSource("src/3", src3);

        sidx = builder.build();
        tmpdir = TempDir::create(TempDir::CLEANUP);
        esa = EnhancedSuffixArray<uint32_t>::build(*sidx, tmpdir->path());
    }

protected:
    TempDir::ptr tmpdir;
    stringstream src1;
    stringstream src2;
    stringstream src3;
    SourceIndexBuilder builder;
    std::shared_ptr<SourceIndex> sidx;
    std::shared_ptr<EnhancedSuffixArray<uint32_t>> esa;
};

TEST_F(TestDuplicationEventFactory, build) {
    Factory factory(*esa);

    std::vector<DuplicationEvent> events;

    std::function<void(LcpInterval const&)> cb =
        [&events, &factory] (LcpInterval const& x) {
            events.push_back(factory.create(x));
        };

    size_t minLcp = 6;
    MaximalIntervalFilter<ArrayType> filter(
        minLcp,
        esa->suffixArray(),
        esa->sourceIndex(),
        cb);

    visitLcpIntervals(esa->lcpArray(), filter);

    typedef DuplicationEvent::Region Rg;
    auto expected = std::vector<DuplicationEvent>{
        DuplicationEvent{11, {
            Rg{"src/2", 0, 4},
            Rg{"src/3", 0, 4},
            },
        },

        DuplicationEvent{10, {
            Rg{"src/1", 0, 4},
            Rg{"src/2", 0, 4},
            Rg{"src/3", 0, 4},
            },
        },
    };

    cout << *esa << "\n";

    EXPECT_EQ(expected, events);
}
