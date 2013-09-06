#include "index/SourceIndex.hpp"

#include "util/TempFile.hpp"

#include <boost/filesystem/path.hpp>
#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
namespace bfs = boost::filesystem;

class TestSourceIndex : public ::testing::Test{
public:
    void SetUp() {
        _fa = "012\n34\n5\n";
        _fb = "678\n9\n";
        _fc = "015\n670\n";

        stringstream positionsJson;
        positionsJson << "{\n "
            << "\"filenames\": [\"a\", \"b\", \"c\"],\n"

            << " \"file_start_positions\": [0, "
                << _fa.size() << ", " << _fa.size() + _fb.size() << "],\n"

            << " \"file_sizes\": [" << _fa.size() << ", "
                << _fb.size() << ", " << _fc.size() << "],"

            << " \"line_start_positions\": [[0, 4, 7], [0, 4], [0, 4]]\n}";

        _sourceData = _fa + _fb + _fc;
        _bs = ByteSource::create(new std::string(_sourceData));
        sidx.reset(new SourceIndex(_bs, positionsJson));
    }

protected:
    std::string _fa;
    std::string _fb;
    std::string _fc;
    std::string _sourceData;
    ByteSource::ptr _bs;
    std::unique_ptr<SourceIndex> sidx;
};

TEST_F(TestSourceIndex, sourceFiles) {
    auto const& sourceFiles = sidx->sourceFiles();
    EXPECT_EQ(3u, sourceFiles.size());

    SourceFile expectedA{"a", // name
        0,                    // start pos
        _fa.size() - 1,       // end pos
        {0, 4, 7}             // line start positions
        };
    EXPECT_EQ(expectedA, sourceFiles[0]);

    SourceFile expectedB{"b",
        _fa.size(),
        _fa.size() + _fb.size() - 1,
        {0, 4}
        };
    EXPECT_EQ(expectedB, sourceFiles[1]);

    SourceFile expectedC{"c",
        _fa.size() + _fb.size(),
        _sourceData.size() - 1,
        {0, 4}
        };
    EXPECT_EQ(expectedC, sourceFiles[2]);

    SourceIndex sidx2(_bs, sourceFiles);
    EXPECT_EQ(sourceFiles, sidx2.sourceFiles());
}

TEST_F(TestSourceIndex, fileAtOffset) {
    SourceFile result;
    for (size_t i = 0; i < _fa.size(); ++i) {
        result = sidx->fileAtOffset(i);
        EXPECT_EQ("a", result.name)
            << "Wrong file at byte " << i << ": " << result;
        EXPECT_EQ(i, result.globalToFileOffset(i))
            << "Wrong offset at byte " << i << ": " << result;
    }

    for (size_t i = _fa.size(); i < _fa.size() + _fb.size(); ++i) {
        result = sidx->fileAtOffset(i);
        EXPECT_EQ("b", result.name)
            << "Wrong file at byte " << i << ": " << result;
        EXPECT_EQ(i - _fa.size(), result.globalToFileOffset(i))
            << "Wrong offset at byte " << i << ": " << result;
    }

    for (size_t i = _fa.size() + _fb.size(); i < _sourceData.size(); ++i) {
        result = sidx->fileAtOffset(i);
        EXPECT_EQ("c", result.name)
            << "Wrong file at byte " << i << ": " << result;
        EXPECT_EQ(i - _fa.size() - _fb.size(), result.globalToFileOffset(i))
            << "Wrong offset at byte " << i << ": " << result;
    }

    EXPECT_THROW(sidx->fileAtOffset(_sourceData.size()), std::runtime_error);
}

TEST_F(TestSourceIndex, lcpOutOfBounds) {
    stringstream mt;
    EXPECT_THROW(sidx->longestCommonPrefix(100, 150), std::runtime_error);
}

TEST_F(TestSourceIndex, data) {
    EXPECT_FALSE(sidx->empty());
    EXPECT_EQ(_sourceData.size(), sidx->size());
    EXPECT_STREQ(_sourceData.data(), sidx->data());
}

TEST_F(TestSourceIndex, lcp) {
    string data(sidx->data(), sidx->size());

    EXPECT_EQ("012", data.substr(0, 3));
    EXPECT_EQ("015", data.substr(15, 3));
    EXPECT_EQ(2u, sidx->longestCommonPrefix(0, 15));

    // Check that we won't compute lcp across a file boundary
    // This crosses the boundary between a & b, (678 is at the start of b)
    EXPECT_EQ("5\n678", data.substr(7, 5));
    EXPECT_EQ("5\n670", data.substr(17, 5));
    // 5\n should match, and then we stop
    EXPECT_EQ(2u, sidx->longestCommonPrefix(7, 17));
}

TEST_F(TestSourceIndex, jsonIo) {
    auto tempDir = TempDir::create(TempDir::CLEANUP);
    auto sourceFile = tempDir->tempFile(TempFile::LEAVE);
    auto posFile = tempDir->tempFile(TempFile::LEAVE);

    sidx->write(sourceFile->stream(), posFile->stream());

    // make sure files are flushed to disk
    sourceFile->stream().close();
    posFile->stream().close();

    SourceIndex newSidx(sourceFile->path(), posFile->path());
    EXPECT_FALSE(newSidx.empty());
    ASSERT_EQ(sidx->size(), newSidx.size());
    EXPECT_TRUE(std::equal(sidx->data(), sidx->data() + sidx->size(),
        newSidx.data()));

    EXPECT_EQ(sidx->sourceFiles(), newSidx.sourceFiles());
}
