#ifndef YAHTML__DRIVER_HH
#define YAHTML__DRIVER_HH

#include <string>
#include <map>
#include "yahtml_parser.hh"
#include "yahtml/DOM.hh"


namespace yahtml {

typedef struct yy_buffer_state * YY_BUFFER_STATE;

class HTMLDriver
{
public:
  DOMChild dom;
  int result;

  std::string file;
  YY_BUFFER_STATE buffer;
  char* src;

  bool trace_scanning;
  bool trace_parsing;

public:
  HTMLDriver();
  HTMLDriver(bool ts, bool tp);
  virtual ~HTMLDriver();

public:
  void parse(const std::string& file);
  void parse_source(const std::string& file);

  void scan_begin();
  void scan_begin_source(const std::string& source);
  void scan_end();
  void scan_end_source();
  void scan_destroy();

  void error(const yahtml::location& l, const std::string& m);
  void error(const std::string& m);
};

}; // !ns yahtml

#endif

