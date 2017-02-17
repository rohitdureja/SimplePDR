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
    namespace AST {
    class ast_node;
    class annotation;
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

%define api.value.type variant
%define parse.assert

/* declare tokens */
%token                      OP CP
%token                      DECLARE_FUN DEFINE_FUN DECLARE_SORT DEFINE_SORT ASSERT
%token                      SET_LOGIC SET_OPTION
%token                      _TRUE _FALSE
%token                      _BOOLEAN _INTEGER
%token                      NEXT INIT INVAR TRANS
%token                      ANNOTATION
%token      <std::string>    IDENTIFIER DEFINE
%token                      AND OR NOT EQUAL
%token                      INDEX
%token                      COMMENT

%type       <std::string>                   type
%type       <AST::ast_node *>               simple_expr expr
%type       <std::vector<AST::ast_node *>>  operands
%type       <AST::annotation *>             annotation

%locations


%%

file            
    : 
    OP statement CP 
    | 
    file OP statement CP
    ;

statement       
    : 
    DECLARE_FUN declaration
    | 
    DEFINE_FUN definition
    | 
    DECLARE_SORT declaration
    | 
    DEFINE_SORT definition
    | 
    ASSERT _TRUE
    ;

declaration     
    : 
    IDENTIFIER OP CP type 
    { 
        driver.mk_var($1); 
    }
    ;

definition
    : 
    DEFINE OP CP type expr 
    { 
        driver.add_definition($1, $5); 
    }                
    ;

type 
    : _BOOLEAN { /* Do nothing */ }
    | _INTEGER { /* Do nothing */ }
    ;

expr
    : 
    OP ANNOTATION simple_expr annotation CP
    { 
        for(unsigned int i = 0 ; i < $4->type.size() ; ++i)
        {
            if($4->type[i] == AST::NEXT)
                driver.add_curr_next($3, $4->next);
            if($4->type[i] == AST::INIT)
                driver.init = $3;
            if($4->type[i] == AST::TRANS)
                driver.trans = $3;
            if($4->type[i] == AST::INVAR)
                driver.invar = $3;
        }
        delete $4;
        $$ = $3; 
    }
    | 
    simple_expr 
    { 
        $$ = $1; 
    }
    ;

annotation
    : { /* Do nothing */}
    | 
    NEXT simple_expr annotation 
    { 
        AST::annotation * a = new AST::annotation();
        a->type.push_back(AST::NEXT);
        a->next = $2;
        $$ = a;
    }
    | 
    INIT _TRUE annotation
    { 
        AST::annotation * a = new AST::annotation();
        a->type.push_back(AST::INIT);
        a->next = nullptr;
        $$ = a;
    }
    | 
    INVAR INDEX annotation
    { 
        AST::annotation * a = new AST::annotation();
        a->type.push_back(AST::INVAR);
        a->next = nullptr;
        $$ = a;
    }
    | 
    TRANS _TRUE annotation
    { 
        AST::annotation * a = new AST::annotation();
        a->type.push_back(AST::TRANS);
        a->next = nullptr;
        $$ = a;
    }
    ;
                

simple_expr     
    : 
    IDENTIFIER
    { 
        $$ = driver.mk_var($1); 
    }
    | 
    DEFINE               
    { 
        $$ = driver.get_ast($1);
    }
    | 
    OP NOT operands CP   
    { 
        $$ = driver.mk_not($3); 
    }
    | 
    OP AND operands CP   
    { 
        $$ = driver.mk_and($3); 
    }
    | 
    OP OR operands CP    
    { 
        $$ = driver.mk_or($3); 
    }
    | 
    OP EQUAL operands CP 
    { 
        $$ = driver.mk_eq($3); 
    }
    |
    _TRUE
    {
        $$ = driver.mk_bool(true);
    }
    |
    _FALSE
    {
        $$ = driver.mk_bool(false);
    }
    ;

operands        
    : 
    simple_expr 
    { 
        $$.push_back($1);
    }
    | 
    operands simple_expr
    { 
        for(unsigned int i = 0 ; i < $1.size() ; ++i)
            $$.push_back($1[i]);
        $$.push_back($2); 
    }
    ;

%%


void Parser::VMT_Parser::error(const location_type &l, 
                          const std::string &err_message) {

    std::cerr << "Error: " << err_message << " at " << l << std::endl;
}
