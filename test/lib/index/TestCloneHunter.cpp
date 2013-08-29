#include "index/CloneHunter.hpp"
#include "index/SourceIndex.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class TestCloneHunter : public ::testing::Test{
public:
    void SetUp() {
        src1 << "This file has one: frog.";
        src2 << "This file has two: frog frog.";
        src3 << "This file has three: frog frog frog.";

        sidx.addSource("src/1", src1);
        sidx.addSource("src/2", src2);
        sidx.addSource("src/3", src3);
    }

protected:
    stringstream src1;
    stringstream src2;
    stringstream src3;
    SourceIndex sidx;
};

TEST_F(TestCloneHunter, gig) {
    CloneHunter<uint32_t> bubba(sidx, 4);
    bubba.goHunting();
    bubba.debugPrintDuplicates();
}


