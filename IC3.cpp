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

IC3::IC3(Model::Model *M) {
//	M->show_init();
    M->show_trans();
    solver = new Solver::Solver();
    map = M->get_var_map();

    for (unsigned int i = 0; i < (*M->get_variables()).size(); ++i) {
        solver->add_symbol((*M->get_variables())[i], Solver::Boolean);
    }

    // get pointer to initial states
    init = M->get_init();

    // get pointer to transition relation
    trans = M->get_trans();

    std::vector<std::string> smt2;
    generate_smtlib2_string(trans, smt2, map);
    for (unsigned int i = 0; i < smt2.size(); ++i) {
        solver->add_assertion(smt2[i]);
    }
    smt2.clear();

//	solver->add_assertion("(assert (= x y))");
//	solver->add_assertion("(assert (= (not x) z))");

	Solver::result res;
	res = solver->check_sat();
	if (res == Solver::sat) {
		std::cout << solver->get_model() << std::endl;
	}

	generate_smtlib2_string(init, smt2, map);
    for (unsigned int i = 0; i < smt2.size(); ++i) {
        solver->add_assertion(smt2[i]);
    }

//  solver->add_assertion("(assert (= x y))");
//  solver->add_assertion("(assert (= (not x) z))");
    res = solver->check_sat();
    if (res == Solver::sat) {
        std::cout << solver->get_model() << std::endl;
    }

//	solver->push(1);
//	solver->add_assertion("(assert (= x z))");
//	res = solver->check_sat();
//	if (res == Solver::sat) {
//		std::cout << solver->get_model() << std::endl;
//	}
//
//	solver->pop(1);
//	solver->add_assertion("(assert (= x y z))");
//	res = solver->check_sat();
//	if (res == Solver::sat) {
//		std::cout << solver->get_model() << std::endl;
//	}
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

bool IC3::prove() {
    // find 0-step counterexample
    solver->push(1);
    solver->add_assertion("(assert (and init (not prop)))");
    if (solver->check_sat() == Solver::sat)
        return false;
    solver->pop(1);

    // first element of frame is init formula
    frames.push_back(init);

    unsigned int k = 1; // active frame number
    frames.push_back(NULL); // add a new frame to the trace

    while (false) {
        // blocking phase
        solver->push(1);
        solver->add_assertion("(assert (and frame[k] (not prop)))");
        while (solver->check_sat() == Solver::sat) {
            std::string model = solver->get_model();
            solver->pop(1);
            if (!rec_block(NULL, k))
                return false;
        }

        // propagation phase
        k = k + 1; // move to next frame
        frames.push_back(NULL); // add a new frame to the trace

        for (unsigned int i = 1; i < k; ++i) {
            std::vector<Clause *> * c = frames[i];
            for (unsigned int j = 0; j < c->size(); ++j) {
                solver->push(1);
                solver->add_assertion(
                        "(assert (and frame[i] c[j] trans (not c[j]')))");
                if (solver->check_sat() == Solver::unsat)
                    frames[i + 1]->push_back((*c)[j]);
            }
            // compare frames[i] and frames[i+1]
        }
    }

    return false;
}

bool IC3::rec_block(Cube * cube, unsigned int) {
    return true;
}
IC3::~IC3() {
    delete solver;
}

} /* namespace IC3 */
