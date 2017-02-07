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

#include "model.h"
#include "utils.h"

namespace Model {

void Model::add_variable(const std::string var_name) {
    var_map1[var_name] = vcount;
    str_vars.push_back(var_name);
    var_map2[vcount] = var_name;
    vcount = vcount + 1;
}

void Model::add_clause(const type ctype,
                       const std::string clause) {
    Clause *c = new Clause();
    std::vector<std::string> parse;
    split(clause, "||", parse);
    for (unsigned char i = 0; i < parse.size(); ++i) {
        if (parse[i][0] == '!') {
            parse[i].erase(parse[i].begin());
            c->add_literal(-var_map1[parse[i]]);
        } else
            c->add_literal(var_map1[parse[i]]);
    }
    if (ctype == T)
        trans.push_back(c);
    else if (ctype == I)
        init.push_back(c);
    else if (ctype == P)
        prop.push_back(c);
    else
        std::cout << "Undefined type!" << std::endl;
}

std::vector<Clause*> * Model::get_trans() {
    return &trans;
}
std::vector<Clause*> * Model::get_init() {
    return &init;
}
std::vector<Clause*> * Model::get_prop() {
    return &prop;
}

void Model::show_trans() {
    std::vector<int8_t> * c;
    for (unsigned char i = 0; i < trans.size(); ++i) {
        c = trans[i]->get_literals();
        for (unsigned char j = 0; j < c->size(); ++j)
            std::cout << ((*c)[j] > 0 ?
                         var_map2[(*c)[j]] :
                         "!" + var_map2[-(*c)[j]]) + " ";
        std::cout << std::endl;
    }
}

void Model::show_init() {
    std::vector<signed char> * c;
    for (unsigned char i = 0; i < init.size(); ++i) {
        c = init[i]->get_literals();
        for (unsigned char j = 0; j < c->size(); ++j)
            std::cout << ((*c)[j] > 0 ?
                         var_map2[(*c)[j]] :
                         "!" + var_map2[-(*c)[j]]) + " ";
        std::cout << std::endl;
    }
}

std::vector<std::string> * Model::get_variables() {
    return &str_vars;
}

std::map<unsigned char, std::string> * Model::get_var_map2() {
    return &var_map2;
}

std::map<std::string, unsigned char> * Model::get_var_map1() {
    return &var_map1;
}

} /* namespace Model */
