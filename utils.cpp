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

#include "utils.h"
#include <iostream>
#include <climits>

void split(const std::string &s, const char* delim,
        std::vector<std::string> & v) {
    char * dup = strdup(s.c_str());
    char * token = std::strtok(dup, delim);
    while (token != NULL) {
        v.push_back(std::string(token));
        token = std::strtok(NULL, delim);
    }
    free(dup);
}

Clause::Clause() {

}

Clause::~Clause() {
    std::cout << "sajhasgasjfgajsfgahsgfas" << std::endl;
    literals.clear();
}

Clause::Clause(Clause * c) {
    std::vector<signed char> lit = c->get_literals();
    for (unsigned int i = 0; i < lit.size(); ++i) {
        signed char l;
        l = lit[i];
        literals.push_back(l);
    }
}

void Clause::add_literal(signed char literal) {
    literals.push_back(literal);
}

std::vector<signed char> Clause::get_literals() {
    return literals;
}

namespace SMTLIB2 {
/* Utility to generate SMTLIB2 strings for each clause in the
 * passed vector
 */
void generate_smtlib2_from_clause(std::vector<std::shared_ptr<Clause>> clauses,
        std::vector<std::string> &str_clause,
        std::map<unsigned char, std::string> * map, smt_str_type type,
        std::map<std::string, std::string> * nmap) {

    if (nmap != NULL) {
        if (type == uncomp) {
            if (clauses.size() == 0) {
                str_clause.push_back("(assert true)");
                return;
            }
            for (unsigned int i = 0; i < clauses.size(); ++i) {
                std::shared_ptr<Clause> clause = clauses[i]; // get i-th clause
                std::vector<signed char> literals = clause->get_literals();
                if (literals.size() == 1) {
                    str_clause.push_back("(assert");
                    str_clause[i] =
                            literals[0] > 0 ?
                                    str_clause[i] + " "
                                            + (*nmap)[(*map)[literals[0]]] :
                                    str_clause[i] + " (not "
                                            + (*nmap)[(*map)[-literals[0]]]
                                            + ")";
                    str_clause[i] = str_clause[i] + ")";
                } else {
                    str_clause.push_back("(assert (or");
                    // iterative over literals in i-th clause
                    for (unsigned int j = 0; j < literals.size(); ++j) {
                        str_clause[i] =
                                literals[j] > 0 ?
                                        str_clause[i] + " "
                                                + (*nmap)[(*map)[literals[j]]] :
                                        str_clause[i] + " (not "
                                                + (*nmap)[(*map)[-literals[j]]]
                                                + ")";
                    }
                    str_clause[i] = str_clause[i] + "))";
                }
            }
        } else if (type == comp) {
            if (clauses.size() == 0) {
                str_clause.push_back("(assert false)");
                return;
            } else if (clauses.size() == 1) {
                std::shared_ptr<Clause> clause = clauses[0]; // get i-th clause
                std::vector<signed char> literals = clause->get_literals();
                if (literals.size() == 1) {
                    str_clause.push_back("(assert");
                    str_clause[0] =
                            literals[0] > 0 ?
                                    str_clause[0] + " (not "
                                            + (*nmap)[(*map)[literals[0]]]
                                            + ")" :
                                    str_clause[0] + " "
                                            + (*nmap)[(*map)[-literals[0]]];

                    str_clause[0] = str_clause[0] + ")";
                } else {
                    str_clause.push_back("(assert (and");
                    // iterative over literals in i-th clause
                    for (unsigned int j = 0; j < literals.size(); ++j) {
                        str_clause[0] =
                                literals[j] > 0 ?
                                        str_clause[0] + " (not "
                                                + (*nmap)[(*map)[literals[j]]]
                                                + ")" :
                                        str_clause[0] + " "
                                                + (*nmap)[(*map)[-literals[j]]];

                    }
                    str_clause[0] = str_clause[0] + "))";
                }
            } else {
                str_clause.push_back("(assert (or");
                // iterate over all clauses
                for (unsigned int i = 0; i < clauses.size(); ++i) {
                    std::shared_ptr<Clause> clause = clauses[i]; // get i-th clause
                    std::vector<signed char> literals = clause->get_literals();
                    if (literals.size() == 1) {
                        str_clause[0] =
                                literals[0] > 0 ?
                                        str_clause[0] + " (not "
                                                + (*nmap)[(*map)[literals[0]]]
                                                + ")" :
                                        str_clause[0] + " "
                                                + (*nmap)[(*map)[-literals[0]]];

                    } else {
                        str_clause[0] = str_clause[0] + " (and";
                        // iterative over literals in i-th clause
                        for (unsigned int j = 0; j < literals.size(); ++j) {
                            str_clause[0] =
                                    literals[j] > 0 ?
                                            str_clause[0] + " (not "
                                                    + (*nmap)[(*map)[literals[j]]]
                                                    + ")" :
                                            str_clause[0] + " "
                                                    + (*nmap)[(*map)[-literals[j]]];

                        }
                        str_clause[0] = str_clause[0] + ")";
                    }
                }
                str_clause[0] = str_clause[0] + "))";
            }
        }
    } else {
        if (type == uncomp) {
            if (clauses.size() == 0) {
                str_clause.push_back("(assert true)");
                return;
            }
            for (unsigned int i = 0; i < clauses.size(); ++i) {
                std::shared_ptr<Clause> clause = clauses[i]; // get i-th clause
                std::vector<signed char> literals = clause->get_literals();
                if (literals.size() == 1) {
                    str_clause.push_back("(assert");
                    str_clause[i] =
                            literals[0] > 0 ?
                                    str_clause[i] + " " + (*map)[literals[0]] :
                                    str_clause[i] + " (not "
                                            + (*map)[-literals[0]] + ")";
                    str_clause[i] = str_clause[i] + ")";
                } else {
                    str_clause.push_back("(assert (or");
                    // iterative over literals in i-th clause
                    for (unsigned int j = 0; j < literals.size(); ++j) {
                        str_clause[i] =
                                literals[j] > 0 ?
                                        str_clause[i] + " "
                                                + (*map)[literals[j]] :
                                        str_clause[i] + " (not "
                                                + (*map)[-literals[j]] + ")";
                    }
                    str_clause[i] = str_clause[i] + "))";
                }
            }
        } else if (type == comp) {
            if (clauses.size() == 0) {
                str_clause.push_back("(assert false)");
                return;
            } else if (clauses.size() == 1) {
                std::shared_ptr<Clause> clause = clauses[0]; // get i-th clause
                std::vector<signed char> literals = clause->get_literals();
                if (literals.size() == 1) {
                    str_clause.push_back("(assert");
                    str_clause[0] =
                            literals[0] > 0 ?
                                    str_clause[0] + " (not "
                                            + (*map)[literals[0]] + ")" :
                                    str_clause[0] + " " + (*map)[-literals[0]];

                    str_clause[0] = str_clause[0] + ")";
                } else {
                    str_clause.push_back("(assert (and");
                    // iterative over literals in i-th clause
                    for (unsigned int j = 0; j < literals.size(); ++j) {
                        str_clause[0] =
                                literals[j] > 0 ?
                                        str_clause[0] + " (not "
                                                + (*map)[literals[j]] + ")" :
                                        str_clause[0] + " "
                                                + (*map)[-literals[j]];

                    }
                    str_clause[0] = str_clause[0] + "))";
                }
            } else {
                str_clause.push_back("(assert (or");
                // iterate over all clauses
                for (unsigned int i = 0; i < clauses.size(); ++i) {
                    std::shared_ptr<Clause> clause = clauses[i]; // get i-th clause
                    std::vector<signed char> literals = clause->get_literals();
                    if (literals.size() == 1) {
                        str_clause[0] =
                                literals[0] > 0 ?
                                        str_clause[0] + " (not "
                                                + (*map)[literals[0]] + ")" :
                                        str_clause[0] + " "
                                                + (*map)[-literals[0]];

                    } else {
                        str_clause[0] = str_clause[0] + " (and";
                        // iterative over literals in i-th clause
                        for (unsigned int j = 0; j < literals.size(); ++j) {
                            str_clause[0] =
                                    literals[j] > 0 ?
                                            str_clause[0] + " (not "
                                                    + (*map)[literals[j]]
                                                    + ")" :
                                            str_clause[0] + " "
                                                    + (*map)[-literals[j]];

                        }
                        str_clause[0] = str_clause[0] + ")";
                    }
                }
                str_clause[0] = str_clause[0] + "))";
            }
        }
    }
}

void generate_clause_from_smtlib2(
        std::vector<std::shared_ptr<Clause>> & clauses,
        std::vector<std::string> cube,
        std::map<std::string, unsigned char> * map) {

    for (unsigned int i = 0; i < cube.size(); ++i) {
        std::shared_ptr<Clause> c(new Clause());
        if (cube[i][0] == '!') {
            cube[i].erase(cube[i].begin());
            c->add_literal(-(*map)[cube[i]]);
        } else {
            c->add_literal((*map)[cube[i]]);
        }
        clauses.push_back(c);
    }
}

std::shared_ptr<Clause> cube_to_clause(std::vector<std::shared_ptr<Clause>> cube) {
    std::shared_ptr<Clause> clause(new Clause());
    for(unsigned int i = 0 ; i < cube.size() ; ++i) {
        std::vector<signed char> lit = cube[i]->get_literals();
        for(unsigned int j = 0 ; j < lit.size() ; ++j) {
            if(lit[j] > 0)
                clause->add_literal(-lit[j]);
            else
                clause->add_literal(lit[j]);
        }
    }
    return clause;
}


} /* namespace SMTLIB2 */

