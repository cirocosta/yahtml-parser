%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"

%define api.prefix {yahtml}
%define api.namespace {yahtml}
%define parser_class_name {HTMLParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
#include <string>
#include "yahtml/Utils.hh"
#include "yahtml/DOM.hh"
}

// passing the parsing context
%param { HTMLDriver& driver }

%locations
%initial-action
{
  // Initialize the initial location
  @$.begin.filename = @$.end.filename = &driver.file;
};

%define parse.trace
%define parse.error verbose

%code
{
#include "yahtml/parser/driver.hh"
#define YY_DECL \
  yahtml::HTMLParser::symbol_type yylex (yahtml::HTMLDriver& driver)

YY_DECL;
}

%define api.token.prefix {HTML_}
%token
  END 0             "End of File (EOF)"
  AB_RIGHT
;
%token <std::string>
  NAME
  TEXT
  START_TAG
  END_TAG
  ATTR_KEY
  ATTR_VALUE
;
%type <DOMChild> dom element;
%type <DOMChildren> value;
%type <AttrMap> attrs;
%type <Attr> attr;

%printer { yyoutput << $$; } <*>;

%%

%start dom;

dom:
  element  { driver.dom = $1; $$ = driver.dom; }
  ;

element
  : START_TAG attrs AB_RIGHT
       value
    END_TAG {
  AttrMap::const_iterator s = $2.find("class");

  if (s != $2.end()) {
    $$ = DOMChild (new Element($1, $2, $4,
                   yahtml::utils::split_str($2["class"])));
  } else {
    $$ = DOMChild (new Element($1, $2, $4));
  }
            }
       ;

attr
  : ATTR_KEY ATTR_VALUE  { $$ = Attr {$1, $2}; }
  ;

attrs
  : %empty       { $$ = AttrMap {}; }
  | attr         { $$ = AttrMap { $1 }; }
  | attrs attr   { $1.emplace($2); $$ = $1; }
  ;

value
  :  %empty         {$$ = DOMChildren { DOMChild (new Text("")) }; }
  |  element        {$$ = DOMChildren { $1 }; }
  |  TEXT           {$$ = DOMChildren { DOMChild (new Text($1)) }; }
  |  value element  {$1.push_back($2); $$ = $1; }
  ;

%%

void yahtml::HTMLParser::error (const location_type& l, const std::string& m)
{
  driver.error(l, m);
}

#undef YY_DECL
