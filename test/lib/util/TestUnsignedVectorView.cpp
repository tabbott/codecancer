#include "util/UnsignedVectorView.hpp"

#include <gtest/gtest.h>

#include <cstdint>

template<typename T>
class TestUnsignedVectorView : public ::testing::Test {
};

TYPED_TEST_CASE_P(TestUnsignedVectorView);
typedef ::testing::Types<
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t
    > NumericTypes;

TYPED_TEST_CASE(TestUnsignedVectorView, NumericTypes);

TYPED_TEST(TestUnsignedVectorView, view) {
    std::vector<TypeParam> x{0, 1, 2, 3, 4};
    UnsignedVectorView view(x);

    EXPECT_EQ(5u, view.size());
    EXPECT_EQ(0u, view[0]);
    EXPECT_EQ(1u, view[1]);
    EXPECT_EQ(2u, view[2]);
    EXPECT_EQ(3u, view[3]);
    EXPECT_EQ(4u, view[4]);
}
