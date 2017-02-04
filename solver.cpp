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

#include "solver.h"
#include <vector>

namespace Solver {

Solver::Solver() {
	c = new z3::context();
	s = new z3::solver(*c);
	nsymbols = 0;
//	Solver::add_symbol(Boolean);
}

void Solver::add_symbol(const std::string symbol, const type tsort) {
	// Create sort vector
	z3::sort_vector sort_v(*c);

	// Add data to vector names and funcs
	// Recommended usage for adding smtlib strings to solver instance
	// Creates objects used by the C API
	names.push_back(c->str_symbol(symbol.c_str()));
	funcs.push_back(c->function(names[nsymbols], sort_v,
			tsort == Boolean ? c->bool_sort() : c->int_sort()));
	// operator Z3_*** are convert types from C to C++
	symbols.push_back(names[nsymbols].operator Z3_symbol());
	decls.push_back(funcs[nsymbols].operator Z3_func_decl());
	nsymbols++;
}

void Solver::add_assertion(const std::string assertion) {
	// Parse the passed SMTLIB2 expression
	Z3_ast parsed1 = Z3_parse_smtlib2_string(*c, assertion.c_str(),
			0, 0, 0, nsymbols, symbols.data(), decls.data());

	// convert the parsed SMTLIB2 (C object) to an expr object for
	// the C++ interface
	z3::expr formula(*c, parsed1);
	s->add(formula);
	std::cout << *s << "\n";
}

result Solver::check_sat() {
	switch (s->check()) {
	case z3::unsat:	std::cout << "unsat" << std::endl; return unsat;
	case z3::sat: std::cout << "sat" << std::endl; return sat;
	default: std::cout << "unknown" << std::endl; return unknown;
	}
}

void Solver::push(const unsigned int step) {
	for (unsigned int i = 0 ; i < step ; ++i)
		s->push();
}

void Solver::pop(const unsigned int step) {
	for (unsigned int i = 0 ; i < step ; ++i)
			s->pop();
}

void Solver::get_model() {
	z3::model m = s->get_model();
	 for (unsigned i = 0; i < m.size(); i++) {
	        z3::func_decl v = m[i];
	        std::cout << v.name() << " = " << m.get_const_interp(v) << "\n";
	    }
}

Solver::~Solver() {
	delete s;
	delete c;
}

} /* namespace Solver */
