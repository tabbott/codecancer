#include "io/ByteSource.hpp"
#include "util/TempFile.hpp"

#include <boost/iostreams/device/mapped_file.hpp>
#include <gtest/gtest.h>

#include <string>

namespace bio = boost::iostreams;

TEST(TestByteSource, byteString) {
    std::string x("abcd");
    ByteSource::ptr bs(ByteSource::create(new std::string(x)));
    EXPECT_EQ(x.size(), bs->size());
    EXPECT_EQ(x, std::string(bs->data(), bs->size()));
}

TEST(TestByteSource, byteVector) {
    std::vector<char> x{'a', 'b', 'c', 'd'};
    ByteSource::ptr bs(ByteSource::create(new std::vector<char>(x)));
    EXPECT_EQ(x.size(), bs->size());
    EXPECT_EQ(std::string(x.data(), x.size()), std::string(bs->data(), bs->size()));
}

TEST(TestByteSource, byteMappedFile) {
    std::string abcd("abcd");
    auto temp = TempFile::create(TempFile::CLEANUP);
    temp->stream() << abcd;
    temp->stream().close();

    ByteSource::ptr bs(ByteSource::create(new bio::mapped_file_source(temp->path())));
    EXPECT_EQ(abcd.size(), bs->size());
    EXPECT_STREQ(abcd.data(), bs->data());
    EXPECT_EQ(abcd, std::string(bs->data(), bs->size()));
}
