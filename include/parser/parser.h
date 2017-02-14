#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>

#include <parser.tab.hh>
#include "scanner.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

namespace Parser{

/* Class to handle AST nodes */
enum otype {AND, OR, NOT, SYM, EQ};
enum atype {INIT, TRANS, NEXT, INVAR, NONE};

class ast_node;
class annotation;

class annotation {
public:
    std::vector<atype> type;
    ast_node * next;
};

class ast_node {
private:
    std::vector<ast> operands;
    otype op;
    std::string id = "";
    std::vector<annotation *> annotations;

public:

    void add_operand(ast ast) {
        operands.push_back(ast);
    }
    void node_type(otype t) {
        op = t;
    }
    void add_name(std::string str) {
        id = str;
    }

    std::vector<ast> get_operands() {
        return operands;
    }

    otype get_ast_type() {
        return op;
    }

    std::string get_ast_string() {
        return id;
    }
};

/* Main driver class */
class VMT_Driver{
public:
   VMT_Driver() {
    
   }

   virtual ~VMT_Driver();
   
   /* parse the given file */
   void parse( const char * const filename );

   /* read from the given file */
   void parse( std::istream &iss );

   /* Helper functions to create ASTs */
   ast mk_var(std::string);
   void mk_expr(ast);
   ast mk_or(std::vector<ast>);
   ast mk_and(std::vector<ast>);
   ast mk_not(std::vector<ast>);
   ast mk_eq(std::vector<ast>);
   ast get_ast(std::string);
   void add_definition(std::string, ast a);

   std::vector<ast> temp;

   /* Accessor functions */
   std::map<std::string, ast> get_var_map();
   std::map<std::string, ast> get_def_map();

   void add_curr_next(ast c, ast n) {
       curr_next_map[c] = n;
   }

   std::map<ast, ast> get_curr_next() {
          return curr_next_map;
      }
   ast trans = nullptr;
   ast init = nullptr;


private:

   /* utility to handle parser expressions */
   void parse_helper( std::istream &stream );

   Parser::VMT_Parser  *parser  = nullptr;
   Parser::VMT_Scanner *scanner = nullptr;

   std::map<std::string, ast> ast_map; // map of definitions
   std::map<std::string, ast> var_map; // map of variables
   std::map<ast, ast> curr_next_map; // map of curr and next states





};
} /* end namespace MC */
#endif /* END __MCDRIVER_HPP__ */
