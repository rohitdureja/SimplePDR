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
#include <sstream>

void split(const std::string &s, const char* delim, std::vector<std::string> & v){
    char * dup = strdup(s.c_str());
    char * token = std::strtok(dup, delim);
    while(token != NULL){
        v.push_back(std::string(token));
        token = std::strtok(NULL, delim);
    }
    free(dup);
}

/**
 *
 */
void Clause::add_literal(signed char literal) {
	literals.push_back(literal);
}

std::vector<signed char> * Clause::get_literals() {
	return &literals;
}

// utility to convert anthing to a string
template<typename T> std::string tostring(const T& x) {
    std::ostringstream os;
    os << x;
    return os.str();
}
