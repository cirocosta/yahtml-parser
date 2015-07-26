#ifndef YAHTML__DOM_HH
#define YAHTML__DOM_HH

#include <typeinfo>
#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <map>
#include <vector>

namespace yahtml {

class HTMLDriver;

class DOM;
class Node;
struct Text;
struct Element;

typedef std::shared_ptr<Node> DOMChild;
typedef std::vector<DOMChild> DOMChildren;
typedef std::vector<std::string> DOMClasses;

struct Attr : std::pair<std::string, std::string>
{
  using std::pair<std::string, std::string>::pair;
};

struct AttrMap : std::map<std::string, std::string>
{
  using std::map<std::string, std::string>::map;
};


// TODO turn this into a 'better DOMChild'.
class DOM
{
public:
  DOM ();
  ~DOM ();
};

enum class NodeType {
  Element, Text
};

class Node
{
public:
  DOMChildren children;
  NodeType type;

  Node(NodeType type, DOMChildren dc = DOMChildren {});
  virtual ~Node();

  virtual void print(std::ostream& where, int ident) const = 0;
};

class Element : public Node
{
public:
  std::string tag_name;
  DOMClasses classes;
  AttrMap attr_map;

  Element(std::string name, AttrMap attrs,
          DOMChildren dc = DOMChildren {},
          DOMClasses classes = DOMClasses {});

  void print(std::ostream& where, int ident) const override;
};


struct Text : public Node
{
  std::string text;

  Text(std::string t);

  void print(std::ostream& where, int ident) const override;
};

std::ostream& operator<<(std::ostream&, const yahtml::Text&);
std::ostream& operator<<(std::ostream&, const yahtml::Node&);
std::ostream& operator<<(std::ostream&, const yahtml::Element&);
std::ostream& operator<<(std::ostream& o, const yahtml::DOMChild& child);
std::ostream& operator<<(std::ostream& o, const yahtml::DOMChildren& children);
std::ostream& operator<<(std::ostream& o, const yahtml::AttrMap& attrmap);
std::ostream& operator<<(std::ostream& o, const yahtml::Attr& attr);

}; // !ns yahtml


#endif
