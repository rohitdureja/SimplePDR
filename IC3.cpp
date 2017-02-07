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
//    M->show_trans();
    solver = new Solver::Solver();

    map1 = M->get_var_map1();
    map2 = M->get_var_map2();

    for (unsigned int i = 0; i < (*M->get_variables()).size(); ++i) {
        solver->add_symbol((*M->get_variables())[i], Solver::Boolean);
    }

    // get pointer to initial states
    init = M->get_init();

    // get pointer to transition relation
    trans = M->get_trans();

    // get pointer to transition relation
    prop = M->get_prop();

//    std::vector<std::string> smt2;
//    generate_smtlib2_string(trans, smt2, map);
//    for (unsigned int i = 0; i < smt2.size(); ++i) {
//        solver->add_assertion(smt2[i]);
//    }
//    smt2.clear();
//
//	Solver::result res;
//	res = solver->check_sat();
//	if (res == Solver::sat) {
//		std::cout << solver->get_model() << std::endl;
//	}
//
//	generate_smtlib2_string(init, smt2, map);
//    for (unsigned int i = 0; i < smt2.size(); ++i) {
//        solver->add_assertion(smt2[i]);
//    }
//
//    res = solver->check_sat();
//    if (res == Solver::sat) {
//        std::cout << solver->get_model() << std::endl;
//    }
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
    // helper string vector to hold SMTLIB2 formulas
    std::vector<std::string> cnf_smt2;

    /***************** find 0-step counterexample ******************
     ************************ starts here **************************/

    solver->push(); // create solver stack

    // get SMT2 string corresponding to init
    SMTLIB2::generate_smtlib2_from_clause(init, cnf_smt2, map2);

#ifdef DEBUG
    std::cout << "IC3::Initial State (size = " << cnf_smt2.size()
              << ")" << std::endl;
    for(unsigned int i = 0 ; i < cnf_smt2.size() ; ++i)
        std::cout << "IC3::" << cnf_smt2[i] << std::endl;
#endif
    // iterate over the SMT2 strings for each clause
    for (unsigned int i = 0; i < cnf_smt2.size(); ++i) {
#ifdef DEBUG
    std::cout << "IC3::Adding constraint " << cnf_smt2[i] << std::endl;
#endif
        solver->add_assertion(cnf_smt2[i]); // add clause to solver
    }
    cnf_smt2.clear(); // clear strings

    // get SMT2 string corresponding to (not prop)
    SMTLIB2::generate_smtlib2_from_clause(prop, cnf_smt2, map2, SMTLIB2::comp);

#ifdef DEBUG
    std::cout << "IC3::Invariant (size = " << cnf_smt2.size()
              << ")" << std::endl;
    for(unsigned int i = 0 ; i < cnf_smt2.size() ; ++i)
        std::cout << "IC3::" << cnf_smt2[i] << std::endl;
#endif
    // iterate over the SMT2 strings for each clause
    for (unsigned int i = 0; i < cnf_smt2.size(); ++i) {
#ifdef DEBUG
    std::cout << "IC3::Adding constraint " << cnf_smt2[i] << std::endl;
#endif
        solver->add_assertion(cnf_smt2[i]); // add clause to solver
    }
    cnf_smt2.clear();

    // checks satisfiability of (I \wedge \not P)
    if (solver->check_sat() == Solver::sat)
        // TODO: extract witness when 0-step cex exists
        return false;
    solver->pop();  // destroy solver stack

    /***************** find 0-step counterexample ******************
     ************************* ends here ***************************/

    // first element of frame is init formula
    frames.push_back(init);

    unsigned int k = 1; // active frame number
    frames.push_back(NULL); // add a new frame to the trace

    //TODO: uncomment this while
//    while (false) {

    /*********************** blocking phase **************************
     ************************* starts here ***************************/
        solver->push(); // create solver stack

        // get SMT2 string corresponding to frames[k]
        SMTLIB2::generate_smtlib2_from_clause(frames[k], cnf_smt2, map2);

#ifdef DEBUG
       std::cout << "IC3::Frame (step = " << k << ")" << std::endl;
       for(unsigned int i = 0 ; i < cnf_smt2.size() ; ++i)
           std::cout << "IC3::" << cnf_smt2[i] << std::endl;
#endif

       // iterate over the SMT2 strings for each clause
       for (unsigned int i = 0; i < cnf_smt2.size(); ++i) {
#ifdef DEBUG
               std::cout << "IC3::Adding constraint " << cnf_smt2[i] << std::endl;
#endif
           solver->add_assertion(cnf_smt2[i]); // add clause to solver
       }
       cnf_smt2.clear(); // clear strings

       // get SMT2 string corresponding to (not prop)
       SMTLIB2::generate_smtlib2_from_clause(prop, cnf_smt2, map2, SMTLIB2::comp);

#ifdef DEBUG
       std::cout << "IC3::Invariant (size = " << cnf_smt2.size()
                 << ")" << std::endl;
       for(unsigned int i = 0 ; i < cnf_smt2.size() ; ++i)
           std::cout << "IC3::" << cnf_smt2[i] << std::endl;
#endif
       // iterate over the SMT2 strings for each clause
       for (unsigned int i = 0; i < cnf_smt2.size(); ++i) {
#ifdef DEBUG
               std::cout << "IC3::Adding constraint " << cnf_smt2[i] << std::endl;
#endif
           solver->add_assertion(cnf_smt2[i]); // add clause to solver
       }
       cnf_smt2.clear();

       // checks satisfiability of (and frames[k] (not prop))
       //TODO: the below "if" needs to be "while"
       if (solver->check_sat() == Solver::sat) {

           // get model corresponding to bad cube c
           std::vector<std::string> bad_cube = solver->get_model();
#ifdef DEBUG
           std::cout << "IC3::Bad cube found!\n";
           for(unsigned int i = 0 ; i < bad_cube.size() ; ++i)
               std::cout << "IC3::" << bad_cube[i] << std::endl;
#endif
           solver->pop(); // destroy solver stack

           // convert bad cube to multiple clauses
           std::vector<Clause *> c;
           SMTLIB2::generate_clause_from_smtlib2(c, bad_cube, map1);

           if (!check_proof_obligation(c, k)) {
               return false;
           }
       }


//        solver->add_assertion("(assert (and frame[k] (not prop)))");
//        while (solver->check_sat() == Solver::sat) {
//            std::string model = solver->get_model();
//            solver->pop(1);
//            if (!rec_block(NULL, k))
//                return false;
//        }
//
//        // propagation phase
//        k = k + 1; // move to next frame
//        frames.push_back(NULL); // add a new frame to the trace
//
//        for (unsigned int i = 1; i < k; ++i) {
//            std::vector<Clause *> * c = frames[i];
//            for (unsigned int j = 0; j < c->size(); ++j) {
//                solver->push(1);
//                solver->add_assertion(
//                        "(assert (and frame[i] c[j] trans (not c[j]')))");
//                if (solver->check_sat() == Solver::unsat)
//                    frames[i + 1]->push_back((*c)[j]);
//            }
//            // compare frames[i] and frames[i+1]
//        }
//    }

    return false;
}

bool IC3::check_proof_obligation(std::vector<Clause *> & s, unsigned int k) {

    // clear memory
    for(unsigned int i = 0 ; i < s.size() ; ++i)
        delete s[i];
    return true;
}
IC3::~IC3() {
    delete solver;
}

} /* namespace IC3 */
