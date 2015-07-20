%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"

%defines
%define parser_class_name {HTMLParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{
#include <string>
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
}

%define api.token.prefix {HTML_}
%token
  END 0             "End of File (EOF)"
  AB_RIGHT          ">"
  SP                " "
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

/* %printer { yyoutput << $$; } <*>; */

%%

%start dom;

dom: element  { driver.dom = $1; $$ = driver.dom; }

element:  START_TAG attrs AB_RIGHT
             value
          END_TAG {
                    $$ = DOMChild (new Element($1, $2, $4));
                  }
       ;

attr: ATTR_KEY ATTR_VALUE     { $$ = std::make_pair($1, $2); }
    | ATTR_KEY ATTR_VALUE     { $$ = std::make_pair($1, $2); }
    ;

attrs: %empty       { $$ = AttrMap {}; }
     | attrs attr   { $1.emplace($2); $$ = $1; }
     ;

value:  %empty            {$$ = DOMChildren { DOMChild (new Text("")) }; }
     |  element           {$$ = DOMChildren { $1 }; }
     |  TEXT              {$$ = DOMChildren { DOMChild (new Text($1)) }; }
     |  value element     {$1.push_back($2); $$ = $1; }
     ;

%%

void yy::HTMLParser::error (const location_type& l, const std::string& m)
{
  driver.error(l, m);
}

