#include "yahtml/DOM.hh"

DOM::DOM () { }
DOM::~DOM () { }

Node::Node(NodeType t, DOMChildren dc) : type(t), children(dc) { }
Node::~Node() { }

std::ostream& operator<<(std::ostream& o, const Node& node)
{
  if (node.type == NodeType::Element) {
    dynamic_cast<const Element*>(&node)->print(o, -1);
  } else {
    dynamic_cast<const Text*>(&node)->print(o, -1);
  }

  return o;
}


Element::Element(std::string name, AttrMap attrs, DOMChildren dc)
  : Node(NodeType::Element, dc), tag_name(name), attr_map(attrs)
{ }

void Element::print(std::ostream& where, int ident) const
{
  int i = ++ident;

  while (i --> 0)
    where << "\t";

  where << "<" << this->tag_name;

  for (auto const &attr : this->attr_map)
    where << attr.first << "=\"" << attr.second << "\" ";

  where << ">" << std::endl;

  for (auto const &child : this->children) {
    child.get()->print(where, ident);
    where << std::endl;
  }

  i = ident;
  while (i --> 0)
    where << "\t";

  where << "</" << this->tag_name << ">";
}

std::ostream& operator<<(std::ostream& o, const Element& e)
{
  return (e.print(o, -1), o);
}


Text::Text(std::string t) : Node(NodeType::Text), text(t) { }

void Text::print(std::ostream& where, int ident) const
{
  int i = ++ident;

  while (i --> 0)
    where << "\t";
  where << this->text;
}

std::ostream& operator<<(std::ostream& o, const Text& text)
{
  return (text.print(o, -1), o);
}

std::ostream& operator<<(std::ostream& o, const DOMChild& child)
{
  o << *(child.get());

  return o;
}

std::ostream& operator<<(std::ostream& o, const DOMChildren& children)
{
  for (const auto &child : children)
    o << child;

  return o;
}
