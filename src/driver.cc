#include "yahtml/parser/driver.hh"

namespace yahtml {

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
  HTMLParser parser(*this);
  parser.set_debug_level(trace_parsing);
  result = parser.parse();
  scan_end();
}

void HTMLDriver::parse_source (const std::string &source)
{
  scan_begin_source(source);
  HTMLParser parser(*this);
  parser.set_debug_level(trace_parsing);
  result = parser.parse();
  scan_end_source();
}

void HTMLDriver::error (const location& l, const std::string& m)
{
  std::cerr << l << ": " << m << std::endl;
}

void HTMLDriver::error (const std::string& m)
{
  std::cerr << m << std::endl;
}

}; // !ns yahtml

