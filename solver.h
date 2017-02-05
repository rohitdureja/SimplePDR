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

#ifndef SOLVER_H_
#define SOLVER_H_
#include "z3++.h"
#include <string>
#include <vector>
#include "utils.h"

namespace Solver {


enum result {sat, unsat, unknown};
enum type {Integer, Boolean};

class Solver {
private:
	z3::context * c;
	z3::solver * s;
	unsigned int nsymbols;
	std::vector<z3::symbol> names;
	std::vector<Z3_symbol> symbols;
	std::vector<z3::func_decl> funcs;
	std::vector<Z3_func_decl> decls;
public:
	Solver();
	virtual ~Solver();
	void push(const unsigned int);
	void pop(const unsigned int);
	void add_symbol(const std::string,const type);
	void add_assertion(const std::string);
	result check_sat();
	std::string get_model();
};

} /* namespace Solver */

#endif /* SOLVER_H_ */
