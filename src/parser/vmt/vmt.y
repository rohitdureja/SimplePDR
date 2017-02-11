%{
#include <stdio.h>
%}

/* declare tokens */
%token OP CP
%token DECLARE_FUN DEFINE_FUN DECLARE_SORT DEFINE_SORT ASSERT
%token SET_LOGIC SET_OPTION
%token _TRUE _FALSE
%token _BOOLEAN _INTEGER
%token NEXT INIT INVAR TRANS
%token ANNOTATION
%token IDENTIFIER DEFINE
%token AND OR NOT
%token INDEX
%token COMMENT

%%

file			: OP statement CP		{ printf ("%s\n", $2); }
				| file OP statement CP
				;

statement		: DECLARE_FUN declaration
				| DEFINE_FUN definition
				| DECLARE_SORT declaration
				| DEFINE_SORT	definition
				| ASSERT _TRUE
				;

declaration		: IDENTIFIER OP CP type
				;

definition		: IDENTIFIER OP CP type expr
				;

type			: _BOOLEAN  { printf ("hello\n"); }
				| _INTEGER
				;

expr			: OP ANNOTATION relation CP
				| simple_expr
				;

relation		: simple_expr asymbol simple_expr
				| simple_expr asymbol
				;

simple_expr		: IDENTIFIER
				| OP NOT IDENTIFIER CP
				| OP AND IDENTIFIER simple_expr CP
				| OR IDENTIFIER simple_expr CP
				;

asymbol			: 
				| NEXT asymbol
				| INIT _TRUE asymbol
				| INVAR INDEX asymbol
				| TRANS _TRUE asymbol
				;

%%

main(int argc, char **argv)
{
	yyparse();
}

yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}