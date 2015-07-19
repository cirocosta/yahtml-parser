#include <string>

#include "gtest/gtest.h"
#include "yahtml/parser/driver.hh"

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

