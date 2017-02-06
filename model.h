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

#ifndef MODEL_H_
#define MODEL_H_

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <map>
#include "utils.h"

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
    std::vector<Clause*> trans;
    std::vector<Clause*> init;
    std::vector<Clause*> prop;

    // Helpers objects
    std::map<std::string, unsigned char> var_map1; //	name -> int
    std::map<unsigned char, std::string> var_map2; //	int -> name
    std::vector<std::string> str_vars;
    unsigned char vcount;

public:

    Model() {
        vcount = 1;
    }
    ~Model() {
        for (unsigned char i = 0; i < trans.size(); ++i)
            delete trans[i];
        for (unsigned char i = 0; i < init.size(); ++i)
            delete init[i];
        for (unsigned char i = 0; i < prop.size(); ++i)
            delete prop[i];
    }

    std::vector<std::string> * get_variables();
    void add_variable(const std::string);
    void add_clause(const type, const std::string);
    std::vector<Clause *> * get_trans();
    std::vector<Clause *> * get_init();
    void show_trans();
    void show_init();
    std::map<unsigned char, std::string> * get_var_map();
};

}

#endif /* MODEL_H_ */
