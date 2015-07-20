#include <string>
#include <sstream>
#include <iostream>
#include <memory>

#include "gtest/gtest.h"
#include "yahtml/DOM.hh"

TEST(Dom, SimpleDomOutput) {
  std::ostringstream actual;
  DOMChild text (new Text("text"));
  DOMChild tag (new Element ("tag", AttrMap {}, DOMChildren {text}));
  DOMChild body (new Element ("body", AttrMap {}, DOMChildren {tag}));

  const char* expected =
    "<body>\n"
      "\t<tag>\n"
        "\t\ttext\n"
      "\t</tag>\n"
    "</body>";

  actual << body;

  EXPECT_EQ(expected, actual.str());
}

TEST(Dom, AttributeOutput) {
  std::ostringstream actual;
  Attr klass {"class", "hue"};
  DOMChild body (new Element ("body", AttrMap {klass}));

  const char* expected =
    "<body class=\"hue\">\n"
    "</body>";

  actual << body;

  EXPECT_EQ(expected, actual.str());
}

TEST(Dom, Simblings) {
  std::ostringstream actual;
  DOMChild text1 (new Text(""));
  DOMChild text2 (new Text(""));

  DOMChild tag1 (new Element ("tag", AttrMap {}, DOMChildren {text1}));
  DOMChild tag2 (new Element ("tag", AttrMap {}, DOMChildren {text2}));
  DOMChild body (new Element ("body", AttrMap {}, DOMChildren {tag1, tag2}));

  const char* expected =
    "<body>\n"
      "\t<tag>\n"
      "\n"
      "\t</tag>\n"
      "\t<tag>\n"
      "\n"
      "\t</tag>\n"
    "</body>";

  actual << body;

  EXPECT_EQ(expected, actual.str());
}

