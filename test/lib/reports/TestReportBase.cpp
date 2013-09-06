#include "reports/ReportBase.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

namespace {
    struct FakeReport : ReportBase {
        void observe(LcpInterval const&) {}
    };
}

TEST(TestReportBase, parseEmptyArgs) {
    FakeReport report;
    EXPECT_NO_THROW(report.parseArgs(std::vector<std::string>()));
}

TEST(TestReportBase, parseNonEmptyArgs) {
    FakeReport report;
    EXPECT_THROW(report.parseArgs(std::vector<std::string>{"a", "b"}),
        std::runtime_error);
}
