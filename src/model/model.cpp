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

#include <model/model.h>
#include <utils/utils.h>

namespace Model {

void Model::read_model(const char * filename) {
    Parser::VMT_Driver driver;
    driver.parse(filename);

    /* set the various model parameters */
    trans = driver.trans;
    init = driver.init;
    prop = driver.invar;

    // get all the variables in the model
    var_map = driver.get_var_map();
    for(std::map<std::string, AST::ast_node *>::iterator it = var_map.begin();
        it != var_map.end();
        ++it) {
        std::string var = it->first;
        add_variable(var);
    }

    // get all the current and next variable relations
    curr_next_map = driver.get_curr_next();
    for(std::map<AST::ast_node *, AST::ast_node *>::iterator it = curr_next_map.begin();
        it != curr_next_map.end();
        ++it) {
        AST::ast_node * curr = it->first;
        AST::ast_node * next = curr_next_map[curr];
        add_variable_relation(curr->get_ast_string(), next->get_ast_string());
    }
}

void Model::add_variable(const std::string var_name) {
    std::cout << var_name << std::endl;
    var_map1[var_name] = vcount;
    str_vars.push_back(var_name);
    var_map2[vcount] = var_name;
    vcount = vcount + 1;
}

void Model::add_variable_relation(const std::string curr,
        const std::string next) {
    var_rel[curr] = next;
}

//void Model::add_clause(const type ctype, const std::string clause) {
//    std::shared_ptr<Clause> c(new Clause());
//    std::vector<std::string> parse;
//    split(clause, "||", parse);
//    for (unsigned char i = 0; i < parse.size(); ++i) {
//        if (parse[i][0] == '!') {
//            parse[i].erase(parse[i].begin());
//            c->add_literal(-var_map1[parse[i]]);
//        } else
//            c->add_literal(var_map1[parse[i]]);
//    }
//    if (ctype == T)
//        trans.push_back(c);
//    else if (ctype == I)
//        init.push_back(c);
//    else if (ctype == P)
//        prop.push_back(c);
//    else
//        std::cout << "Undefined type!" << std::endl;
//}

AST::ast_node * Model::get_trans() {
    return trans;
}
AST::ast_node * Model::get_init() {
    return init;
}
AST::ast_node * Model::get_prop() {
    return prop;
}

void Model::show_trans() {
    std::string tr;
    generate_string(tr, trans);
    std::cout << "Transition Relation: \n" << tr << std::endl;
}

void Model::show_init() {
    std::string in;
    generate_string(in, init);
    std::cout << "Initial State(s): \n" << in << std::endl;
}

std::vector<std::string> Model::get_variables() {
    return str_vars;
}

std::map<unsigned char, std::string> * Model::get_var_map2() {
    return &var_map2;
}

std::map<std::string, unsigned char> * Model::get_var_map1() {
    return &var_map1;
}

std::map<std::string, std::string> * Model::get_var_rel_map() {
    return &var_rel;
}

} /* namespace Model */
