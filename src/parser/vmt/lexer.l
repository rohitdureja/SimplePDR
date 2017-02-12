%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include <parser/scanner.h>
#undef  YY_DECL
#define YY_DECL int Parser::VMT_Scanner::yylex( Parser::VMT_Parser::semantic_type * const lval, Parser::VMT_Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = Parser::VMT_Parser::token;

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option yylineno
%option debug
%option nodefault
%option yyclass="Parser::VMT_Scanner"
%option noyywrap
%option c++

%%
^"(".*"set-info".*")"$          { /* ignore first line */ }
"set-logic"                     { return token::SET_LOGIC; }
"set-option"                    { return token::SET_OPTION; }
"declare-sort"                  { return token::DECLARE_SORT; }
"define-sort"                   { return token::DEFINE_SORT; }
"declare-fun"                   { return token::DECLARE_FUN; }
"define-fun"                    { return token::DEFINE_FUN; }
"assert"                        { return token::ASSERT; }
"("                             { return token::OP; }
")"                             { return token::CP; }
":next"                         { return token::NEXT ; }
":init"                         { return token::INIT ; }
":trans"                        { return token::TRANS ; }
":invar-property"               { return token::INVAR ; }
[0-9]+                          { return token::INDEX ; }
"Int"                           { return token::_INTEGER ; }
"Bool"                          { return token::_BOOLEAN ; }
";".*                           { return token::COMMENT ; }
"!"                             { return token::ANNOTATION ; }
"or"                            { return token::OR ; }
"not"                           { return token::NOT ; }
"and"                           { return token::AND ; }
"="				                { return token::EQUAL; }
"true"                          { return token::_TRUE ; }
"false"                         { return token::_FALSE ; }
".def_"[0-9]+                   { return token::DEFINE ; }
[a-zA-Z0-9\.\_]+                { return token::IDENTIFIER ; }
\n                              { /* ignore newline */ }
" "                             { /* ignore whitespace */ }
.                               { return 0;/* process default */ }

%%