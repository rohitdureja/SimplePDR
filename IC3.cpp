/*
 * IC3.cpp
 *
 *  Created on: Jan 30, 2017
 *      Author: dureja
 */

#include "IC3.h"

namespace IC3 {

IC3::IC3(Model *M) {
	M->show_init();
	M->show_trans();
}

bool IC3::prove() {
	std::cout << "de-Morgan example\n";
	z3::context c;
	z3::solver s(c);

	z3::sort_vector sort_v(c);
	Z3_symbol symbols[2];
	z3::symbol x = c.str_symbol("x");
	z3::symbol y = c.str_symbol("y");

	Z3_func_decl decls[2];
	z3::func_decl funx = c.function(x, sort_v , c.bool_sort());
	z3::func_decl funy = c.function(y, sort_v , c.bool_sort());


	symbols[0] = x.operator Z3_symbol();
	symbols[1] = y.operator Z3_symbol();

	decls[0] = funx.operator Z3_func_decl();
	decls[1] = funy.operator Z3_func_decl();


	std::string conjectur = "(assert (= x y))";
	Z3_ast parsed1 = Z3_parse_smtlib2_string(c, (Z3_string) conjectur.c_str(), 0, 0, 0, 2, symbols, decls);
	z3::expr conjecture1(c, parsed1);

	// adding the negation of the conjecture as a constraint.
	s.add(!conjecture1);
	s.push();
	std::string conjectur3 = "(assert (or x y))";
		Z3_ast parsed3 = Z3_parse_smtlib2_string(c, (Z3_string) conjectur3.c_str(), 0, 0, 0, 2, symbols, decls);
		z3::expr conjecture3(c, parsed3);
		s.add(!conjecture3);
	std::cout << s << "\n";
//	std::cout << s.to_smt2() << "\n";
	switch (s.check()) {
	case z3::unsat:   std::cout << "de-Morgan is valid\n"; break;
	case z3::sat:     std::cout << "de-Morgan is not valid\n"; break;
	case z3::unknown: std::cout << "unknown\n"; break;
	}
	s.pop();
	conjectur3 = "(assert (and x y))";
	Z3_ast parsed2 = Z3_parse_smtlib2_string(c, (Z3_string) conjectur3.c_str(), 0, 0, 0, 2, symbols, decls);
	z3::expr conjecture2(c, parsed2);

		// adding the negation of the conjecture as a constraint.
		s.add(!conjecture2);
		std::cout << s << "\n";
//		std::cout << s.to_smt2() << "\n";
		switch (s.check()) {
		case z3::unsat:   std::cout << "de-Morgan is valid\n"; break;
		case z3::sat:     std::cout << "de-Morgan is not valid\n"; break;
		case z3::unknown: std::cout << "unknown\n"; break;
		}

	return true;
}

IC3::~IC3() {
	// TODO Auto-generated destructor stub
}

} /* namespace IC3 */
