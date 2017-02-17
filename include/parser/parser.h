/*************************************************************************
 * Copyright (C) 2017 by Rohit Dureja                                    *
 *                                                                       *
 * This file is part of SimplePDR.                                       *
 *                                                                       *
 *  SimplePDR is free software: you can redistribute it and/or modify    *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  SimplePDR is distributed in the hope that it will be useful,         *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with SimplePDR.  If not, see <http://www.gnu.org/licenses/>.   *
 *************************************************************************/

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "scanner.h"
#include <utils/ast.h>

namespace Parser {

/* Main driver class */
class VMT_Driver {
public:
    VMT_Driver() {
    }

    virtual ~VMT_Driver();

    /* parse the given file */
    void parse(const char * const filename);

    /* read from the given file */
    void parse(std::istream &iss);

    /* Helper functions to create ASTs */
    AST::ast_node * mk_var(std::string);
    void mk_expr(AST::ast_node *);
    AST::ast_node * mk_or(std::vector<AST::ast_node *>);
    AST::ast_node * mk_and(std::vector<AST::ast_node *>);
    AST::ast_node * mk_not(std::vector<AST::ast_node *>);
    AST::ast_node * mk_eq(std::vector<AST::ast_node *>);
    AST::ast_node * mk_bool(bool);
    AST::ast_node * get_ast(std::string);
    void add_definition(std::string, AST::ast_node * a);

    /* Accessor functions */
    std::map<std::string, AST::ast_node *> get_var_map();
    std::map<std::string, AST::ast_node *> get_def_map();

    /* Add current and next version of a symbol */
    void add_curr_next(AST::ast_node * c, AST::ast_node * n) {
        curr_next_map[c] = n;
    }

    /* Retrive map of current and next version of symbols */
    std::map<AST::ast_node *, AST::ast_node *> get_curr_next() {
        return curr_next_map;
    }

    /* Model read from VMT File */
    AST::ast_node * trans = nullptr;
    AST::ast_node * init = nullptr;
    AST::ast_node * invar = nullptr;

private:

    /* utility to handle parser expressions */
    void parse_helper(std::istream &stream);

    /* Pointer to the scanner and parser */
    Parser::VMT_Parser *parser = nullptr;
    Parser::VMT_Scanner *scanner = nullptr;

    std::map<std::string,
             AST::ast_node *> ast_map; // map of definitions

    std::map<std::string,
             AST::ast_node *> var_map; // map of variables

    std::map<AST::ast_node *,
             AST::ast_node *> curr_next_map; // map of curr and next states
};
} /* end namespace Parser */
#endif /* END __PARSER_HPP__ */
