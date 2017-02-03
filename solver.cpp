/*
 * Solver.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: dureja
 */

#include "solver.h"
#include <vector>

namespace Solver {

Solver::Solver() {
	c = new z3::context();
	s = new z3::solver(*c);
	Solver::add_symbol(Boolean);
}

void Solver::add_symbol(type tsort) {

	z3::sort_vector sort_v(*c);
		Z3_symbol symbols[2];
		std::vector<z3::symbol> names;
		names.push_back(c->str_symbol("x"));
		names.push_back(c->str_symbol("y"));

		Z3_func_decl decls[2];
		std::vector<z3::func_decl> funcs;
		funcs.push_back(c->function(names[0], sort_v , c->bool_sort()));
		funcs.push_back(c->function(names[1], sort_v , c->bool_sort()));

		symbols[0] = names[0].operator Z3_symbol();
		symbols[1] = names[1].operator Z3_symbol();

		decls[0] = funcs[0].operator Z3_func_decl();
		decls[1] = funcs[1].operator Z3_func_decl();


		std::string conjectur = "(assert (= x y))";
		Z3_ast parsed1 = Z3_parse_smtlib2_string(*c, (Z3_string) conjectur.c_str(), 0, 0, 0, names.size(), symbols, decls);
		std::cout << Z3_ast_to_string(*c, parsed1);
		z3::expr conjecture1(*c, parsed1);

		// adding the negation of the conjecture as a constraint.
		s->add(!conjecture1);
//		s.push();
		std::string conjectur3 = "(assert (or x y))";
		Z3_ast parsed3 = Z3_parse_smtlib2_string(*c, (Z3_string) conjectur3.c_str(), 0, 0, 0, names.size(), symbols, decls);
		std::cout << Z3_ast_to_string(*c, parsed3);
//		z3::expr conjecture3(c, parsed3);
//		s.add(!conjecture3);
		std::cout << *s << "\n";
		//	std::cout << s.to_smt2() << "\n";
		switch (s->check()) {
		case z3::unsat:   std::cout << "de-Morgan is valid\n"; break;
		case z3::sat:     std::cout << "de-Morgan is not valid\n"; break;
		case z3::unknown: std::cout << "unknown\n"; break;
		}
//		s.pop();
//		conjectur3 = "(assert (and x y))";
//		Z3_ast parsed2 = Z3_parse_smtlib2_string(c, (Z3_string) conjectur3.c_str(), 0, 0, 0, names.size(), symbols, decls);
//		z3::expr conjecture2(c, parsed2);
//
//			// adding the negation of the conjecture as a constraint.
//			s.add(!conjecture2);
//			std::cout << s << "\n";
//		//		std::cout << s.to_smt2() << "\n";
//			switch (s.check()) {
//			case z3::unsat:   std::cout << "de-Morgan is valid\n"; break;
//			case z3::sat:     std::cout << "de-Morgan is not valid\n"; break;
//			case z3::unknown: std::cout << "unknown\n"; break;
//			}
}

Solver::~Solver() {
	delete s;
	delete c;
}

} /* namespace Solver */
