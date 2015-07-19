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

TEST(Html, NestedWithSimblings) {
  bool debug = true;
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

