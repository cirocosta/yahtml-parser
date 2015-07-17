#include "yahtml/parser/driver.hh"
#include "parser.hh"

HTMLDriver::HTMLDriver ()
  : trace_scanning(false), trace_parsing(false)
{
}

HTMLDriver::HTMLDriver (bool ts, bool tp)
  : trace_scanning(ts), trace_parsing(tp)
{
}

HTMLDriver::~HTMLDriver()
{
  scan_destroy();
}

void HTMLDriver::parse (const std::string &f)
{
  file = f;
  scan_begin();
  yy::HTMLParser parser(*this);
  parser.set_debug_level(trace_parsing);
  result = parser.parse();
  scan_end();
}

void HTMLDriver::parse_source (const std::string &source)
{
  scan_begin_source(source);
  yy::HTMLParser parser(*this);
  parser.set_debug_level(trace_parsing);
  result = parser.parse();
  scan_end_source();
}

void HTMLDriver::error (const yy::location& l, const std::string& m)
{
  std::cerr << l << ": " << m << std::endl;
}

void HTMLDriver::error (const std::string& m)
{
  std::cerr << m << std::endl;
}

