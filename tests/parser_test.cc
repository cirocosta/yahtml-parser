#include <string>

#include "gtest/gtest.h"
#include "yahtml/parser/driver.hh"

TEST(Dummy, DummyTest) {
  bool debug = true;
  HTMLDriver driver (debug, debug);
  driver.parse_source("<body><hue></hue></body>");

  EXPECT_EQ(1, 1);
}

