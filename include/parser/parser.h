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

typedef ast_node * ast;

class ast_node {
private:
    std::vector<ast> operands;
    otype op;
    std::string id = "";

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
   ast add_ast(std::string);

private:

   /* utility to handle parser expressions */
   void parse_helper( std::istream &stream );

   Parser::VMT_Parser  *parser  = nullptr;
   Parser::VMT_Scanner *scanner = nullptr;

   std::map<ast, ast> curr_next_map;

   std::map<std::string, ast> ast_map;


};
} /* end namespace MC */
#endif /* END __MCDRIVER_HPP__ */
