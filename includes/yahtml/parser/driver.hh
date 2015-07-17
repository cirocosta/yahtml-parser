#ifndef YAHTML__DRIVER_HH
#define YAHTML__DRIVER_HH

#include <string>
#include <map>
#include "parser.hh"
#include "yahtml/DOM.hh"

#define YY_DECL \
  yy::HTMLParser::symbol_type yylex (HTMLDriver& driver)

YY_DECL;

typedef struct yy_buffer_state * YY_BUFFER_STATE;

/**
 * Public Interface to the HTTP Parser
 */
class HTMLDriver
{
public:
  DOM dom;
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

  void error(const yy::location& l, const std::string& m);
  void error(const std::string& m);
};

#endif