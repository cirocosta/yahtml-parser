#include <string>
#include <iostream>
#include <memory>

#include "gtest/gtest.h"
#include "yahtml/DOM.hh"

TEST(Dummy, DummyTest) {
  // driver.parse_source("<body><hue></hue></body>");

  DOMChild text (new Text("huee"));
  DOMChild paragraph (new Element ("p", AttrMap {}, DOMChildren {text}));
  DOMChild body (new Element ("body", AttrMap {}, DOMChildren {paragraph}));

  std::cout << body << std::endl;

  EXPECT_EQ(1, 1);
}

