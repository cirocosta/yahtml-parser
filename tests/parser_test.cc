#include <string>

#include "gtest/gtest.h"
#include "yahtml/parser/driver.hh"

using namespace yahtml;

TEST(Html, SingleH1) {
  bool debug = false;
  HTMLDriver driver (debug, debug);
  driver.parse_source("<h1 data-name=\"NAME\"></h1>");

  EXPECT_EQ(driver.dom.get()->type, NodeType::Element);
  EXPECT_EQ(driver.dom.get()->children.size(), 1);

  Element* h1 = dynamic_cast<Element*>(driver.dom.get());

  EXPECT_EQ(h1->tag_name, "h1");
  EXPECT_EQ(h1->attr_map.size(), 1);
  EXPECT_EQ(h1->attr_map["data-name"], "NAME");
  EXPECT_EQ(h1->children.size(), 1);
  EXPECT_EQ(h1->children.front().get()->type, NodeType::Text);
}

TEST(Html, NestedWithoutText) {
  bool debug = false;
  HTMLDriver driver (debug, debug);
  driver.parse_source("<body><tag></tag></body>");

  EXPECT_EQ(driver.dom.get()->children.size(), 1);
  EXPECT_EQ(driver.dom.get()->children.front().get()->type,
            NodeType::Element);

  Element* body = dynamic_cast<Element*>(driver.dom.get());

  EXPECT_EQ(body->tag_name, "body");
  EXPECT_EQ(body->attr_map.size(), 0);
  EXPECT_EQ(body->children.size(), 1);
  EXPECT_EQ(body->children.front().get()->type, NodeType::Element);

  Element* tag = dynamic_cast<Element*>(body->children.front().get());

  EXPECT_EQ(tag->tag_name, "tag");
  EXPECT_EQ(tag->attr_map.size(), 0);
  EXPECT_EQ(tag->children.size(), 1);
}

TEST(Html, NestedWithText) {
  bool debug = false;
  HTMLDriver driver (debug, debug);
  driver.parse_source("<body><tag>text</tag></body>");

  EXPECT_EQ(driver.dom.get()->children.size(), 1);
  EXPECT_EQ(driver.dom.get()->children.front().get()->type,
            NodeType::Element);

  Element* body = dynamic_cast<Element*>(driver.dom.get());

  EXPECT_EQ(body->tag_name, "body");
  EXPECT_EQ(body->attr_map.size(), 0);
  EXPECT_EQ(body->children.size(), 1);
  EXPECT_EQ(body->children.front().get()->type, NodeType::Element);

  Element* tag = dynamic_cast<Element*>(body->children.front().get());

  EXPECT_EQ(tag->tag_name, "tag");
  EXPECT_EQ(tag->attr_map.size(), 0);
  EXPECT_EQ(tag->children.size(), 1);
  EXPECT_EQ(tag->children.front().get()->type, NodeType::Text);

  Text* text = dynamic_cast<Text*>(tag->children.front().get());

  EXPECT_EQ(text->text, "text");
}

TEST(Html, NestedWithSimblings) {
  bool debug = false;
  HTMLDriver driver (debug, debug);
  const char* source =
    "<body>"
      "<tag>"
      "</tag>"
      "<tag>"
      "</tag>"
    "</body>"
    "";
  driver.parse_source(source);

  EXPECT_EQ(driver.dom.get()->type, NodeType::Element);

  Element* body = dynamic_cast<Element*>(driver.dom.get());

  EXPECT_EQ(body->tag_name, "body");
  EXPECT_EQ(body->attr_map.size(), 0);
  EXPECT_EQ(body->children.size(), 2);

  EXPECT_EQ(body->children[0]->type, NodeType::Element);
  EXPECT_EQ(body->children[1]->type, NodeType::Element);

  Element* tag1 = dynamic_cast<Element*>(body->children[0].get());
  Element* tag2 = dynamic_cast<Element*>(body->children[1].get());

  EXPECT_EQ(tag1->tag_name, "tag");
  EXPECT_EQ(tag1->attr_map.size(), 0);
  EXPECT_EQ(tag1->children.size(), 1);
  EXPECT_EQ(tag1->children.front().get()->type, NodeType::Text);

  EXPECT_EQ(tag2->tag_name, "tag");
  EXPECT_EQ(tag2->attr_map.size(), 0);
  EXPECT_EQ(tag2->children.size(), 1);
  EXPECT_EQ(tag2->children.front().get()->type, NodeType::Text);
}

TEST(Html, NestedWithDifferentSimblings) {
  bool debug = false;
  HTMLDriver driver (debug, debug);
  const char* source =
    "<body>"
      "<tag>"
      "</tag>"
      "<p>"
      "</p>"
    "</body>"
    "";
  driver.parse_source(source);

  EXPECT_EQ(driver.dom.get()->type, NodeType::Element);

  Element* body = dynamic_cast<Element*>(driver.dom.get());

  EXPECT_EQ(body->tag_name, "body");
  EXPECT_EQ(body->attr_map.size(), 0);
  EXPECT_EQ(body->children.size(), 2);

  EXPECT_EQ(body->children[0]->type, NodeType::Element);
  EXPECT_EQ(body->children[1]->type, NodeType::Element);

  Element* tag1 = dynamic_cast<Element*>(body->children[0].get());
  Element* tag2 = dynamic_cast<Element*>(body->children[1].get());

  EXPECT_EQ(tag1->tag_name, "tag");
  EXPECT_EQ(tag1->attr_map.size(), 0);
  EXPECT_EQ(tag1->children.size(), 1);
  EXPECT_EQ(tag1->children.front().get()->type, NodeType::Text);

  EXPECT_EQ(tag2->tag_name, "p");
  EXPECT_EQ(tag2->attr_map.size(), 0);
  EXPECT_EQ(tag2->children.size(), 1);
  EXPECT_EQ(tag2->children.front().get()->type, NodeType::Text);
}

TEST(Html, FlatWithOneAttribute) {
  bool debug = false;
  HTMLDriver driver(debug, debug);
  const char* source =
    "<body class=\"hue\">"
    "</body>"
    "";
  driver.parse_source(source);

  EXPECT_EQ(driver.dom.get()->type, NodeType::Element);

  Element* body = dynamic_cast<Element*>(driver.dom.get());

  EXPECT_EQ(body->tag_name, "body");
  EXPECT_EQ(body->attr_map.size(), 1);
  EXPECT_EQ(body->attr_map["class"], "hue");
}

TEST(Html, NestedMultipleAttributes) {
  bool debug = false;
  HTMLDriver driver(debug, debug);
  const char* source =
    "<body class=\"hue\" id=\"cool\">"
      "<tag class=\"hue\">"
      "</tag>"
      "<tag class=\"hue\">"
        "huehue brbr"
      "</tag>"
    "</body>"
    "";
  driver.parse_source(source);

  EXPECT_EQ(driver.dom.get()->type, NodeType::Element);

  Element* body = dynamic_cast<Element*>(driver.dom.get());

  EXPECT_EQ(body->tag_name, "body");

  EXPECT_EQ(body->attr_map.size(), 2);
  EXPECT_EQ(body->attr_map["class"], "hue");
  EXPECT_EQ(body->attr_map["id"], "cool");

  EXPECT_EQ(body->children.size(), 2);
  EXPECT_EQ(body->children[0]->type, NodeType::Element);
  EXPECT_EQ(body->children[1]->type, NodeType::Element);


  Element* tag1 = dynamic_cast<Element*>(body->children[0].get());
  Element* tag2 = dynamic_cast<Element*>(body->children[1].get());

  EXPECT_EQ(tag1->attr_map.size(), 1);
  EXPECT_EQ(tag2->attr_map.size(), 1);
  EXPECT_EQ(tag1->attr_map["class"], "hue");
  EXPECT_EQ(tag2->attr_map["class"], "hue");

  EXPECT_EQ(tag2->children.front().get()->type, NodeType::Text);


  Text* text = dynamic_cast<Text*>(tag2->children.front().get());
  EXPECT_EQ(text->text, "huehue brbr");
}

TEST(Html, EmptyClassAttribute) {
  bool debug = false;
  HTMLDriver driver(debug, debug);
  const char* source =
    "<body class=\"\">"
    "</body>"
    "";
  driver.parse_source(source);

  EXPECT_EQ(driver.dom.get()->type, NodeType::Element);

  Element* body = dynamic_cast<Element*>(driver.dom.get());

  EXPECT_EQ(body->tag_name, "body");
  EXPECT_EQ(body->attr_map.size(), 1);
  EXPECT_EQ(body->attr_map["class"], "");
  EXPECT_EQ(body->classes.size(), 0);
}

TEST(Html, MultipleClassesAttribute) {
  bool debug = false;
  HTMLDriver driver(debug, debug);
  const char* source =
    "<body class=\"hue br lol\">"
    "</body>"
    "";
  driver.parse_source(source);

  EXPECT_EQ(driver.dom.get()->type, NodeType::Element);

  Element* body = dynamic_cast<Element*>(driver.dom.get());

  EXPECT_EQ(body->tag_name, "body");
  EXPECT_EQ(body->attr_map.size(), 1);
  EXPECT_EQ(body->attr_map["class"], "hue br lol");

  EXPECT_EQ(body->classes[0], "hue");
  EXPECT_EQ(body->classes[1], "br");
  EXPECT_EQ(body->classes[2], "lol");
}

TEST(Html, ElementWithSimblingText) {
  bool debug = false;
  HTMLDriver driver(debug, debug);
  const char* source =
    "<body>"
      "<h1></h1>"
      "huehue <strong>brbr</strong> huehue"
    "</body>";

  driver.parse_source(source);
  ASSERT_EQ(driver.result, 0);
}

