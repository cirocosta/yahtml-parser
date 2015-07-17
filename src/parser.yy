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
  END 0 "End of File (EOF)"
;
%token <std::string> TAG ENDTAG TEXT
%type <DOMChild> dom value;

%printer { yyoutput << $$; } <*>;
%%
%start dom;

dom: TAG value ENDTAG {$$ = DOMChild (new Element($1, AttrMap {})); }
   ;

value:  dom           {$$ = $1; }
     |  TEXT          {$$ = DOMChild (new Text($1)); }
     |  value dom     {$$ = DOMChild (new Element("a", AttrMap{})); }
     ;

%%

void yy::HTMLParser::error (const location_type& l, const std::string& m)
{
  driver.error(l, m);
}

