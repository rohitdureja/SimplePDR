/*
 * ast.h
 *
 *  Created on: Feb 16, 2017
 *      Author: dureja
 */

#ifndef INCLUDE_UTILS_AST_H_
#define INCLUDE_UTILS_AST_H_
#include <vector>
#include <string>

namespace AST {
/* Class to handle AST nodes */
enum otype {
    AND, OR, NOT, SYM, EQ, BOOL
};
enum atype {
    INIT, TRANS, NEXT, INVAR, NONE
};

/* class to manage AST node */
class ast_node {
private:
    /* vector containing node operands */
    std::vector<ast_node *> operands;

    /* type of AST node */
    otype op;

    /* name if node is a symbol node */
    std::string id = "";

    /* boolean value if node is BOOL type */
    bool val;

public:
    /* destructor for AST node */
    ~ast_node();

    /* add operands to AST node */
    void add_operand(ast_node * ast);

    /* configure type of node */
    void node_type(otype t);

    /* add name if node is of type SYM */
    void add_name(std::string str) ;

    /* add value if node is of type BOOL */
    void add_val(bool v);

    /* get operands of the AST node */
    std::vector<ast_node *> get_operands();

    /* get the type of AST node */
    otype get_ast_type();

    /* get the name if node is of type SYM */
    std::string get_ast_string();

    /* get the value if the node is of type BOOL */
    bool get_ast_val();
};

/* friend class to keep track of definition annotation */
class annotation {
public:
    std::vector<atype> type;
    ast_node * next;
};

}

#endif /* INCLUDE_UTILS_AST_H_ */
