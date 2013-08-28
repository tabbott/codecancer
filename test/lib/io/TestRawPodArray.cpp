#include "io/PodArray.hpp"
#include "util/TempFile.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template<typename T>
class TestPodArray : public ::testing::Test {
};

struct u128 {
    uint64_t x;
    uint64_t y;
};

TYPED_TEST_CASE_P(TestPodArray);
typedef ::testing::Types<
    int8_t, uint8_t,
    int16_t, uint16_t,
    int32_t, uint32_t,
    int64_t, uint64_t
    > NumericTypes;

TYPED_TEST_CASE(TestPodArray, NumericTypes);

TYPED_TEST(TestPodArray, writeAndMap) {
    std::vector<TypeParam> numbers{1, 2, 3, 4, 5};
    std::stringstream out;
    writePodArray(out, numbers.data(), numbers.size(),
        &identityTransform<TypeParam>);
    std::string data = out.str();

    EXPECT_THROW(MappedPodArray<u128>(data.data(), data.size()), std::runtime_error);

    MappedPodArray<TypeParam> mapped(data.data(), data.size());
    EXPECT_EQ(5u, mapped.size());
    EXPECT_EQ(TypeParam(1), mapped[0]);
    EXPECT_EQ(TypeParam(2), mapped[1]);
    EXPECT_EQ(TypeParam(3), mapped[2]);
    EXPECT_EQ(TypeParam(4), mapped[3]);
    EXPECT_EQ(TypeParam(5), mapped[4]);
}

TYPED_TEST(TestPodArray, writeAndMapTransformed) {
    std::vector<TypeParam> numbers{1, 2, 3, 4, 5};
    std::stringstream out;

    // apply transformation f(x) -> x + 1 before writing
    writePodArray(out, numbers.data(), numbers.size(),
        [] (TypeParam x) {
            return x + 1;
        });

    std::string data = out.str();

    MappedPodArray<TypeParam> mapped(data.data(), data.size());
    EXPECT_EQ(5u, mapped.size());
    EXPECT_EQ(TypeParam(2), mapped[0]);
    EXPECT_EQ(TypeParam(3), mapped[1]);
    EXPECT_EQ(TypeParam(4), mapped[2]);
    EXPECT_EQ(TypeParam(5), mapped[3]);
    EXPECT_EQ(TypeParam(6), mapped[4]);
}
