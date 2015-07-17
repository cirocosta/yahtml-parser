%{ /* -*- C++ -*- */

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "yahtml/parser/driver.hh"
#include "parser.hh"

#undef yywrap
#define yywrap() 1

// The location of the current token.
static yy::location loc;

%}

%option noyywrap nounput batch debug noinput

ALPHA               [\x41-\x5A\x61-\x7A]

TAG                 "<"{ALPHA}+">"
ENDTAG              "</"{ALPHA}+">"
NUM                 [0-9]+
STR                 [^><]+

%{
  // Code run each time a pattern is matched.
  #define YY_USER_ACTION  loc.columns(yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step();
%}

{TAG}           return yy::HTMLParser::make_TAG(yytext, loc);

{ENDTAG}        return yy::HTMLParser::make_ENDTAG(yytext, loc);

{NUM}           {
                  errno = 0;
                  long n = std::strtol(yytext, NULL, 10);

                  if (!(INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
                    driver.error(loc, "Integer out of range.");

                  return yy::HTMLParser::make_NUM(n, loc);
                }

{STR}           return yy::HTMLParser::make_STR(yytext, loc);

.               driver.error(loc, "Invalid Character");

<<EOF>>         return yy::HTMLParser::make_END(loc);

%%

void HTMLDriver::scan_begin_source (const std::string& source)
{
  src = new char[source.size() + 1];
  std::copy(source.begin(), source.end(), src);
  src[source.size()] = '\0';

  yy_flex_debug = trace_scanning;
  buffer = yy_scan_string(src);
}

void HTMLDriver::scan_end_source ()
{
  yy_delete_buffer(buffer);
  delete[] src;
}


void HTMLDriver::scan_begin ()
{
  yy_flex_debug = trace_scanning;

  if (!(yyin = fopen(file.c_str(), "r"))) {
    error("Can't open " + file + ": " + strerror(errno));
    exit(EXIT_FAILURE);
  }

  buffer = yy_create_buffer(yyin, YY_BUF_SIZE);
  yy_switch_to_buffer(buffer);
}

void HTMLDriver::scan_destroy ()
{
  yylex_destroy();
}

void HTMLDriver::scan_end ()
{
  yy_flush_buffer(buffer);
  yy_delete_buffer(buffer);
  fclose(yyin);
}

