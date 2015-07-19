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
  AB_LEFT           "<"
  AB_RIGHT          ">"
  AB_LEFT_CLOSED    "</"
;
%token <std::string> NAME TEXT START_TAG END_TAG
%type <DOMChild> element value;

%printer { yyoutput << $$; } <*>;

%%

%start dom;

dom: element  { driver.dom = $1; }

element:  START_TAG AB_RIGHT
             value
          END_TAG {
        $$ = DOMChild (new Element($1, AttrMap {}, DOMChildren {$3}));
      }
   ;

value:  element           {$$ = $1; }
     |  TEXT          {$$ = DOMChild (new Text($1)); }
     |  value element     {$$ = DOMChild (new Element("a", AttrMap{})); }
     ;

%%

void yy::HTMLParser::error (const location_type& l, const std::string& m)
{
  driver.error(l, m);
}

