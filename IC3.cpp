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

#include "IC3.h"

namespace IC3 {

IC3::IC3(Model *M) {
	M->show_init();
	M->show_trans();
	solver = new Solver::Solver();

	for (unsigned int i = 0; i < (*M->get_variables()).size(); ++i) {
		solver->add_symbol((*M->get_variables())[i], Solver::Boolean);
	}

	solver->add_assertion("(assert (= x y))");
	solver->add_assertion("(assert (= (not x) z))");

	Solver::result res;
	res = solver->check_sat();
	if (res == Solver::sat) {
		std::cout << solver->get_model() << std::endl;
	}

	solver->push(1);
	solver->add_assertion("(assert (= x z))");
	res = solver->check_sat();
	if (res == Solver::sat) {
		std::cout << solver->get_model() << std::endl;
	}

	solver->pop(1);
	solver->add_assertion("(assert (= x y z))");
	res = solver->check_sat();
	if (res == Solver::sat) {
		std::cout << solver->get_model() << std::endl;
	}
}

/*
 * IC3 algorithm description is described in:
 * Griggio, Alberto, and Marco Roveri. "Comparing different variants of the
 * IC3 algorithm for hardware model checking." IEEE Transactions on
 * Computer-Aided Design of Integrated Circuits and Systems 35.6 (2016).
 *
 * 	bool IC3(I,T,P):
 * 		if is_sat(I & !P): return False;
 * 		F[0] = I # first element of trace is init formula
 * 		k = 1, F[k] = T # add a new frame to the trace
 * 		while True:
 * 			# blocking phase
 * 			while is_sat(F[k] & P):
 * 				c = get_model() # extract bad cube c
 * 				if not rec_block(c,k):
 * 					return False # counterexample found
 *
 * 			# propagation phase
 * 			k = k + 1, F[k] = T
 * 			for i = 1 to k - 1:
 * 				for each clause c \in F[i]:
 * 					if not is_sat(F[i] & c & T & !c'):
 * 						add c to F[i+1]
 * 				if F[i] == F[i+1]: return True
 *
 */

// TODO: Implement data structure to maintain frames
bool IC3::prove() {
		return true;
}

IC3::~IC3() {
	delete solver;
}

} /* namespace IC3 */
