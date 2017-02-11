%option noyywrap
%option nodefault
%{
#include <stdio.h>
#include "vmt.tab.h"
%}

%%
^"(".*"set-info".*")"$		{ /* ignore first line */ }
"set-logic"					{ return SET_LOGIC; }
"set-option"				{ return SET_OPTION; }
"declare-sort"				{ return DECLARE_SORT; }
"define-sort"				{ return DEFINE_SORT; }
"declare-fun"				{ return DECLARE_FUN; }
"define-fun"				{ return DEFINE_FUN; }
"assert"					{ return ASSERT; }
"("							{ return OP; }
")"							{ return CP; }
":next"						{ return NEXT ; }
":init"						{ return INIT ; }
":trans"					{ return TRANS ; }
":invar-property"			{ return INVAR ; }
[0-9]+						{ return INDEX ; }
"Int"						{ return _INTEGER ; }
"Bool"						{ return _BOOLEAN ; }
";".*						{ return COMMENT ; }
"!"							{ return ANNOTATION ; }
"or"						{ return OR ; }
"not"						{ return NOT ; }
"and"						{ return AND ; }
"true"						{ return _TRUE ; }
"false"						{ return _FALSE ; }
".def_"[0-9]+				{ return DEFINE ; }
[a-zA-Z0-9\.\_]+			{ return IDENTIFIER ; }
\n 							{ /* ignore newline */ }
" "							{ /* ignore whitespace */ }
.							{ return 0;/* process default */ }
%%
