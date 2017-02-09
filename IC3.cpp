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
#include <memory>
#include <set>

namespace IC3 {

IC3::IC3(std::shared_ptr<Model::Model> M) {
//	M->show_init();
//    M->show_trans();
    solver = new Solver::Solver();

    map1 = M->get_var_map1();
    map2 = M->get_var_map2();
    nmap = M->get_var_rel_map();

    for (unsigned int i = 0; i < (M->get_variables()).size(); ++i) {
        solver->add_symbol((M->get_variables())[i], Solver::Boolean);
    }

    // get pointer to initial states
    init = M->get_init();

    // get pointer to transition relation
    trans = M->get_trans();

    // get pointer to transition relation
    prop = M->get_prop();

}

/************************************************************************
 * IC3 ``main'' algorithm description is described in:                  *
 * Griggio, Alberto, and Marco Roveri. "Comparing different variants    *
 * of the IC3 algorithm for hardware model checking." IEEE              *
 * Transactions on Computer-Aided Design of Integrated Circuits and     *
 * Systems 35.6 (2016).                                                 *
 *                                                                      *
 * 	bool IC3(I,T,P):                                                    *
 * 		if is_sat(I & !P): return False;                                *
 * 		F[0] = I # first element of trace is init formula               *
 * 		k = 1, F[k] = T # add a new frame to the trace                  *
 * 		while True:                                                     *
 * 			# blocking phase                                            *
 * 			while is_sat(F[k] & P):                                     *
 * 				c = get_model() # extract bad cube c                    *
 * 				if not rec_block(c,k):                                  *
 * 					return False # counterexample found                 *
 *                                                                      *
 * 			# propagation phase                                         *
 * 			k = k + 1, F[k] = T                                         *
 * 			for i = 1 to k - 1:                                         *
 * 				for each clause c \in F[i]:                             *
 * 					if not is_sat(F[i] & c & T & !c'):                  *
 * 						add c to F[i+1]                                 *
 * 				if F[i] == F[i+1]: return True                          *
 *                                                                      *
 ************************************************************************/

bool IC3::prove() {
    // helper string vector to hold SMTLIB2 formulas
    std::vector<std::string> cnf_smt2;

    /***************** find 0-step counterexample ******************
     ************************ starts here **************************/

    solver->push(); // create solver stack

    // get SMT2 string corresponding to init
    SMTLIB2::generate_smtlib2_from_clause(init, cnf_smt2, map2, SMTLIB2::uncomp,
    NULL);

#ifdef DEBUG
    std::cout << "IC3::Initial State (size = " << cnf_smt2.size()
    << ")" << std::endl;
    for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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
    SMTLIB2::generate_smtlib2_from_clause(prop, cnf_smt2, map2, SMTLIB2::comp,
    NULL);

#ifdef DEBUG
    std::cout << "IC3::Invariant (size = " << cnf_smt2.size()
    << ")" << std::endl;
    for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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
    std::vector<std::shared_ptr<Clause>> m;
    frames.push_back(m); // add a new frame to the trace

    while (true) {

        /*********************** blocking phase **************************
         ************************* starts here ***************************/

        solver->push(); // create solver stack

#ifdef DEBUG
        std::cout << "\n-------Starting blocking phase------" << std::endl;
        std::cout << "------------------------------------\n" << std::endl;
#endif

        // get SMT2 string corresponding to frames[k]
        SMTLIB2::generate_smtlib2_from_clause(frames[k], cnf_smt2, map2,
                SMTLIB2::uncomp, NULL);

#ifdef DEBUG
        std::cout << "IC3::Frame (step = " << k << ")" << std::endl;
        for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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
        SMTLIB2::generate_smtlib2_from_clause(prop, cnf_smt2, map2,
                SMTLIB2::comp,
                NULL);

#ifdef DEBUG
        std::cout << "IC3::Invariant (size = " << cnf_smt2.size()
        << ")" << std::endl;
        for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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
        while (solver->check_sat() == Solver::sat) {

            // get model corresponding to bad cube c
            std::vector<std::string> bad_cube = solver->get_model();
#ifdef DEBUG
            std::cout << "IC3::Bad cube found!\n";
            for(unsigned int i = 0; i < bad_cube.size(); ++i)
            std::cout << "IC3::" << bad_cube[i] << std::endl;
#endif
            solver->pop(); // destroy solver stack

            // convert bad cube to multiple clauses
            std::vector<std::shared_ptr<Clause>> c;
            SMTLIB2::generate_clause_from_smtlib2(c, bad_cube, map1, nmap->size());

            if (!check_proof_obligation(c, k)) {
                return false;
            }

            solver->push(); // create solver stack

            // get SMT2 string corresponding to frames[k]
            SMTLIB2::generate_smtlib2_from_clause(frames[k], cnf_smt2, map2,
                    SMTLIB2::uncomp, NULL);

#ifdef DEBUG
            std::cout << "IC3::Frame (step = " << k << ")" << std::endl;
            for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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
            SMTLIB2::generate_smtlib2_from_clause(prop, cnf_smt2, map2,
                    SMTLIB2::comp,
                    NULL);

#ifdef DEBUG
            std::cout << "IC3::Invariant (size = " << cnf_smt2.size()
            << ")" << std::endl;
            for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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
        }
        solver->pop();

        /*********************** blocking phase **************************
         ************************** ends here ****************************/

        /********************** propagation phase ************************
         ************************* starts here ***************************/

#ifdef DEBUG
        std::cout << "\n-----Starting propagation phase-----" << std::endl;
        std::cout << "------------------------------------\n" << std::endl;
#endif

        k = k + 1;
        std::vector<std::shared_ptr<Clause>> n;
        frames.push_back(n); // add a new frame to the trace

        /* New solver stack created since trans is always present
         * in subsequent calls to Z3 (next for loop) */
        solver->push(); // create solver stack

        // get SMT2 string corresponding to trans
        SMTLIB2::generate_smtlib2_from_clause(trans, cnf_smt2, map2,
                SMTLIB2::uncomp, NULL);

#ifdef DEBUG
        std::cout << "IC3::Transition relation (size = " << cnf_smt2.size() << ")" << std::endl;
        for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

        // iterate over frames
        for (unsigned int i = 1; i < k; ++i) {
            std::vector<std::shared_ptr<Clause>> cl = frames[i];

            /* New solver stack created since frames[i] is present
             * in all iterations of the next for loop */
            solver->push(); // create solver stack

            // get SMT2 string corresponding to frames[i] = cl
            SMTLIB2::generate_smtlib2_from_clause(cl, cnf_smt2, map2,
                    SMTLIB2::uncomp, NULL);

#ifdef DEBUG
            std::cout << "IC3::Frame (step = " << i << ")" << std::endl;
            for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

            // Iterate over all clauses in frames[i];
            for (unsigned j = 0; j < cl.size(); ++j) {
                /* New solver stack created for adding cl[i] to solver */
                solver->push(); // create solver stack

                // get SMT2 string corresponding to cl[j]
                std::vector<std::shared_ptr<Clause>> m; // = new std::vector<Clause *>;
                m.push_back(cl[j]);
                SMTLIB2::generate_smtlib2_from_clause(m, cnf_smt2, map2,
                        SMTLIB2::uncomp, NULL);

#ifdef DEBUG
                std::cout << "IC3::clause[" << j << "] in frames[" << i << "]" << std::endl;
                for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

                // get SMT2 string corresponding to (not cl[j]')
                SMTLIB2::generate_smtlib2_from_clause(m, cnf_smt2, map2,
                        SMTLIB2::comp, nmap);

#ifdef DEBUG
                std::cout << "IC3::(not clause[" << j << "]') in frames[" << i << "]" << std::endl;
                for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

                // propagate forward from frames[i] to frames[i+1]
                if (solver->check_sat() == Solver::unsat) {
                    frames[i + 1].push_back(m[0]);
                }

                /* Solver stack destroyed to remove cl[i] */
                solver->pop(); // destroy solver stack
            }
            /* Solver stack destroyed to remove frame[i] */
            solver->pop(); // destroy solver stack

            std::set<std::shared_ptr<Clause>> myset1;
            std::set<unsigned long> myset2;
            for (unsigned int j = 0; j < frames[i].size(); ++j) {
//                std::cout << "Frame " << i << ": " << frames[i][j] << std::endl;
                myset1.insert(frames[i][j]);
            }

            for (unsigned int j = 0; j < frames[i + 1].size(); ++j) {
//                std::cout <<  "Frame " << i+1 << ": " << frames[i + 1][j] << std::endl;
                myset1.erase(frames[i+1][j]);
            }
//            std::cout << myset1.size() << std::endl;
            if(myset1.size() == 0)
                return true;

        }
        /* Solver stack destroyed to remove trans */
        solver->pop(); // destroy solver stack

        /********************** propagation phase ************************
         ************************** ends here ****************************/
    }
    return true;
}

bool IC3::check_proof_obligation(std::vector<std::shared_ptr<Clause>> s,
        unsigned int k) {

#ifdef DEBUG
    std::cout << "\n-------Check proof obligation-------" << std::endl;
    std::cout << "------------------------------------\n" << std::endl;
#endif

    if (k == 0)
        return false; // reached initial state

    std::vector<std::string> cnf_smt2; // helper string vector to hold SMTLIB2 formulas

    solver->push(); // create solver stack

    // get SMT2 string corresponding to trans
    SMTLIB2::generate_smtlib2_from_clause(trans, cnf_smt2, map2,
            SMTLIB2::uncomp, NULL);

#ifdef DEBUG
    std::cout << "IC3::Transition relation (size = " << cnf_smt2.size() << ")" << std::endl;
    for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

    std::vector<std::shared_ptr<Clause>> cl = frames[k - 1];

    // get SMT2 string corresponding to frames[k-1]
    SMTLIB2::generate_smtlib2_from_clause(cl, cnf_smt2, map2, SMTLIB2::uncomp,
    NULL);

#ifdef DEBUG
    std::cout << "IC3::Frame (step = " << k-1 << ")" << std::endl;
    for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

    // get SMT2 string corresponding to (not s)
    SMTLIB2::generate_smtlib2_from_clause(s, cnf_smt2, map2, SMTLIB2::comp,
    NULL);

#ifdef DEBUG
    std::cout << "IC3::bad cube (not s)" << std::endl;
    for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

    // get SMT2 string corresponding to (s')
    SMTLIB2::generate_smtlib2_from_clause(s, cnf_smt2, map2, SMTLIB2::uncomp,
            nmap);

#ifdef DEBUG
    std::cout << "IC3::bad cube s'" << std::endl;
    for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

    while (solver->check_sat() == Solver::sat) {

        // get model corresponding to bad cube t
        std::vector<std::string> bad_cube = solver->get_model();

#ifdef DEBUG
        std::cout << "IC3::Bad cube found!\n";
        for(unsigned int i = 0; i < bad_cube.size(); ++i)
        std::cout << "IC3::" << bad_cube[i] << std::endl;
#endif

        solver->pop(); // destroy solver stack

        // convert bad cube to multiple clauses (containing curr states only)
        std::vector<std::shared_ptr<Clause>> t;
        SMTLIB2::generate_clause_from_smtlib2(t, bad_cube, map1, nmap->size());

        if (!check_proof_obligation(t, k - 1)) {
            return false;
        }

        solver->push();

        // get SMT2 string corresponding to trans
        SMTLIB2::generate_smtlib2_from_clause(trans, cnf_smt2, map2,
                SMTLIB2::uncomp, NULL);

#ifdef DEBUG
        std::cout << "IC3::Transition relation (size = " << cnf_smt2.size() << ")" << std::endl;
        for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

        std::vector<std::shared_ptr<Clause>> cl = frames[k - 1];

        // get SMT2 string corresponding to frames[k-1]
        SMTLIB2::generate_smtlib2_from_clause(cl, cnf_smt2, map2,
                SMTLIB2::uncomp,
                NULL);

#ifdef DEBUG
        std::cout << "IC3::Frame (step = " << k-1 << ")-------------------------5" << std::endl;
        for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

        // get SMT2 string corresponding to (not s)
        SMTLIB2::generate_smtlib2_from_clause(s, cnf_smt2, map2, SMTLIB2::comp,
        NULL);

#ifdef DEBUG
        std::cout << "IC3::bad cube (not s)" << std::endl;
        for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

        // get SMT2 string corresponding to (s')
        SMTLIB2::generate_smtlib2_from_clause(s, cnf_smt2, map2,
                SMTLIB2::uncomp, nmap);

#ifdef DEBUG
        std::cout << "IC3::bad cube s'" << std::endl;
        for(unsigned int i = 0; i < cnf_smt2.size(); ++i)
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

    }

    solver->pop();

    std::shared_ptr<Clause> b(SMTLIB2::cube_to_clause(s));

    for (unsigned int i = 1; i <= k; ++i) {
        frames[i].push_back(b);
    }
    return true;
}

IC3::~IC3() {
    delete solver;
}

} /* namespace IC3 */
