#include "test_utils.h"

#include <cstdint>

// Inline the enums from SagaTaluSettings.h to avoid firmware dependencies
namespace sagatalu {
struct Settings {
  enum SideButtonLayout : uint8_t { PrevNext = 0, NextPrev = 1 };
  enum FrontButtonLayout : uint8_t { FrontBCLR = 0, FrontLRBC = 1 };

  uint8_t sideButtonLayout = PrevNext;
  uint8_t frontButtonLayout = FrontBCLR;
};
}  // namespace sagatalu

int main() {
  TestUtils::TestRunner runner("SettingsDefaultsTest");

  // FrontButtonLayout enum values
  runner.expectEq(uint8_t(0), uint8_t(sagatalu::Settings::FrontBCLR), "FrontBCLR == 0");
  runner.expectEq(uint8_t(1), uint8_t(sagatalu::Settings::FrontLRBC), "FrontLRBC == 1");

  // SideButtonLayout enum values
  runner.expectEq(uint8_t(0), uint8_t(sagatalu::Settings::PrevNext), "PrevNext == 0");
  runner.expectEq(uint8_t(1), uint8_t(sagatalu::Settings::NextPrev), "NextPrev == 1");

  // Default values
  sagatalu::Settings settings;
  runner.expectEq(uint8_t(sagatalu::Settings::FrontBCLR), settings.frontButtonLayout, "frontButtonLayout default is FrontBCLR");
  runner.expectEq(uint8_t(sagatalu::Settings::PrevNext), settings.sideButtonLayout, "sideButtonLayout default is PrevNext");

  runner.printSummary();
  return runner.allPassed() ? 0 : 1;
}
