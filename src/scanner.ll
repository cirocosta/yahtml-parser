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

ALPHA               [a-zA-Z]
NAME                {ALPHA}+
TEXT                [^><]+

ATTR_KEY            " "{NAME}"="
ATTR_VALUE          "\""([^\"]|\\.)*"\""

AB_RIGHT            ">"
START_TAG           "<"{NAME}
END_TAG             "</"{NAME}">"

%{
  // Code run each time a pattern is matched.
  #define YY_USER_ACTION  loc.columns(yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step();
%}

{START_TAG}     return yy::HTMLParser::make_START_TAG(yytext+1, loc);

{END_TAG}       {
                  return yy::HTMLParser::make_END_TAG(
                      std::string(yytext, 2, yyleng-3), loc);
                }

{ATTR_KEY}      return yy::HTMLParser::make_ATTR_KEY(yytext, loc);

{ATTR_VALUE}    return yy::HTMLParser::make_ATTR_VALUE(yytext, loc);

{TEXT}          return yy::HTMLParser::make_TEXT(yytext, loc);

">"             return yy::HTMLParser::make_AB_RIGHT(loc);

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

