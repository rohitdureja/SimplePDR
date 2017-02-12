%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {Parser}
%define parser_class_name {VMT_Parser}

%code requires{
    namespace Parser {
    class VMT_Driver;
    class VMT_Scanner;
    }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#   if defined __cplusplus && 201103L <= __cplusplus
#       define YY_NULLPTR nullptr
#   else
#       define YY_NULLPTR 0
#   endif
# endif

}

%parse-param { VMT_Scanner  &scanner  }
%parse-param { VMT_Driver  &driver  }

%code{

#include <iostream>
#include <cstdlib>
#include <fstream>
   
/* include for all driver functions */
#include <parser/parser.h>

#undef yylex
#define yylex scanner.yylex

}

/* declare tokens */
%token OP CP
%token DECLARE_FUN DEFINE_FUN DECLARE_SORT DEFINE_SORT ASSERT
%token SET_LOGIC SET_OPTION
%token _TRUE _FALSE
%token _BOOLEAN _INTEGER
%token NEXT INIT INVAR TRANS
%token ANNOTATION
%token IDENTIFIER DEFINE
%token AND OR NOT EQUAL
%token INDEX
%token COMMENT

%locations

%%

file            : OP statement CP   { std::cout << "hello\n"; }
                | file OP statement CP
                ;

statement       : DECLARE_FUN declaration
                | DEFINE_FUN definition
                | DECLARE_SORT declaration
                | DEFINE_SORT definition
                | ASSERT _TRUE
                ;

declaration     : IDENTIFIER OP CP type
                ;

definition      : DEFINE OP CP type expr
                ;

type            : _BOOLEAN  { std::cout << "hello\n"; }
                | _INTEGER
                ;

expr            : OP ANNOTATION simple_expr annotation CP
                | simple_expr
                ;

annotation		:
				| NEXT simple_expr annotation
				| INIT _TRUE annotation
				| INVAR INDEX annotation
				| TRANS _TRUE annotation
				;
				

simple_expr     : IDENTIFIER { std::cout << "hello\n"; }
                | DEFINE { std::cout << "hello\n"; }
                | OP NOT simple_expr CP
                | OP AND simple_expr simple_expr CP
                | OP OR simple_expr simple_expr CP
                | OP EQUAL simple_expr simple_expr CP
                | _TRUE
                | _FALSE
                ;

%%


void Parser::VMT_Parser::error(const location_type &l, 
                          const std::string &err_message) {

    std::cerr << "Error: " << err_message << " at " << l << std::endl;
}
