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

#ifndef IC3_H_
#define IC3_H_

#include "model.h"
#include "solver.h"
#include <vector>
#include "utils.h"

namespace IC3 {

class IC3 {
private:
    // Z3 solver instance
    Solver::Solver * solver;

    /* Keep track of IC3 frames:
     * Data structure is a vector of pointers to a vector of pointers
     * to clauses.
     *
     * Reference use:
     * frames.push_back(M->get_init());         // push init to frame[0]
     *
     * std::vector<Clause *> * c = frames[0];   // get vector containing
     * 									   	    // pointers to clauses of
     * 									   	    // frame[0]
     *
     * std::cout << c->size() << std::endl;
     * for (unsigned int j = 0 ; j <c->size() ; ++j) {
     * 	Clause * f = (*c)[j];                   // get j-th of clause of
     * 	                                        // frame[0]
     *
     * 	std::vector<signed char> * lit;         // pointer to a vector of
     * 	                                        // literals.
     *
     * 	lit = f->get_literals();                // make lit point to literals in
     * 	                                        // j-th clause of frame[0]
     *
     * 	for (unsigned int i = 0 ; i < lit->size() ; ++i)
     * 	std::cout << (*lit)[i] << std::endl;
     * }
     */
    std::vector<std::vector<Clause *> *> frames;
    std::vector<Clause *> * init;
    std::vector<Clause *> * trans;
    std::map<unsigned char, std::string> * map;

    bool rec_block(Cube *, unsigned int);
public:
    IC3(Model::Model *);
    bool prove();
    virtual ~IC3();
};

} /* namespace IC3 */

#endif /* IC3_H_ */
