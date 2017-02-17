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

#ifndef INCLUDE_MODEL_MODEL_H_
#define INCLUDE_MODEL_MODEL_H_

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <memory>

#include <utils/utils.h>
#include <parser/parser.h>

namespace Model {

enum type {
    I, T, P
};
/*
 * ``Model'' class to maintain attributes of the transition system
 * Maintains the set of variables, the transition system (in CNF),
 * initial state (in CNF).
 */
class Model {
    /*
     * Vector containing pointer to clauses clauses for the
     * transition system.
     */
    AST::ast_node * trans = nullptr;
    AST::ast_node * init = nullptr;
    AST::ast_node * prop = nullptr;

    std::map<std::string, AST::ast_node *> var_map; // variables
    std::map<AST::ast_node *, AST::ast_node *> curr_next_map; // curr -> next

    // Helpers objects
    std::map<std::string, unsigned char> var_map1; //	name -> int
    std::map<unsigned char, std::string> var_map2; //	int -> name
    std::map<std::string, std::string> var_rel; //   curr -> next
    std::vector<std::string> str_vars;
    unsigned char vcount;

    void add_variable(const std::string);
    void add_variable_relation(const std::string, const std::string);

public:

    Model() {
        vcount = 1;
    }

    void read_model(const char *);

    std::vector<std::string> get_variables();

//    void add_clause(const type, const std::string);
    AST::ast_node * get_trans();
    AST::ast_node * get_init();
    AST::ast_node * get_prop();
    void show_trans();
    void show_init();
    std::map<unsigned char, std::string> * get_var_map2();
    std::map<std::string, unsigned char> * get_var_map1();
    std::map<std::string, std::string> * get_var_rel_map();
};

}

#endif /* INCLUDE_MODEL_MODEL_H_ */
