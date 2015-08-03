%{ /* -*- C++ -*- */

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "yahtml/parser/driver.hh"
#include "yahtml_parser.hh"

#undef yywrap
#define yywrap() 1

#define YY_DECL \
  yahtml::HTMLParser::symbol_type yylex (yahtml::HTMLDriver& driver)

YY_DECL;

// The location of the current token.
static yahtml::location loc;

%}

%option noyywrap nounput batch debug noinput
%option prefix="yahtml"

%x TAG

ALPHA               [a-zA-Z]
DIGIT               [0-9]
NAME                {ALPHA}({ALPHA}|{DIGIT}|"-")*
TEXT                [^><]+
SP                  [ \t\r\n]

ATTR_KEY            " "{NAME}"="
ATTR_VALUE          \"([^\"]|\\.)*\"

DOCTYPE             "<!doctype"{SP}+"html"{SP}*">"{SP}*
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

{SP}*"<!--"[^>]*"-->"{SP}* { }

(?i:{DOCTYPE})  return yahtml::HTMLParser::make_DOCTYPE(loc);

{START_TAG}     {
                  BEGIN(TAG);
                  return yahtml::HTMLParser::make_START_TAG(yytext+1, loc);
                }

<TAG>{AB_RIGHT} {
                  BEGIN(INITIAL);
                  return yahtml::HTMLParser::make_AB_RIGHT(loc);
                }

<TAG>{ATTR_KEY} {
                  return yahtml::HTMLParser::make_ATTR_KEY(
                        std::string(yytext, 1, yyleng-2), loc);
                }

<TAG>{ATTR_VALUE} {
                    return yahtml::HTMLParser::make_ATTR_VALUE(
                        std::string(yytext, 1, yyleng-2), loc);
                  }

{END_TAG}       {
                  return yahtml::HTMLParser::make_END_TAG(
                      std::string(yytext, 2, yyleng-3), loc);
                }

{TEXT}          return yahtml::HTMLParser::make_TEXT(yytext, loc);


.               driver.error(loc, "Invalid Character");

<<EOF>>         return yahtml::HTMLParser::make_END(loc);

%%

void yahtml::HTMLDriver::scan_begin_source (const std::string& source)
{
  src = new char[source.size() + 1];
  std::copy(source.begin(), source.end(), src);
  src[source.size()] = '\0';

  yy_flex_debug = trace_scanning;
  buffer = yy_scan_string(src);
}

void yahtml::HTMLDriver::scan_end_source ()
{
  yy_delete_buffer(buffer);
  delete[] src;
}


void yahtml::HTMLDriver::scan_begin ()
{
  yy_flex_debug = trace_scanning;

  if (!(yyin = fopen(file.c_str(), "r"))) {
    error("Can't open " + file + ": " + strerror(errno));
    exit(EXIT_FAILURE);
  }

  buffer = yy_create_buffer(yyin, YY_BUF_SIZE);
  yy_switch_to_buffer(buffer);
}

void yahtml::HTMLDriver::scan_destroy ()
{
  yylex_destroy();
}

void yahtml::HTMLDriver::scan_end ()
{
  yy_flush_buffer(buffer);
  yy_delete_buffer(buffer);
  fclose(yyin);
}

#undef YY_DECL
