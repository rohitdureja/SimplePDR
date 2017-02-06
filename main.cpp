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

#include "model.h"
#include "IC3.h"

int main() {
    // Specify transition system
    Model *M = new Model();
    M->add_variable("x");
    M->add_variable("y");
    M->add_variable("z");
    M->add_variable("nx");
    M->add_variable("ny");
    M->add_variable("nz");
    M->add_clause("trans", "!x|z");
    M->add_clause("trans", "!y|!z");
    M->add_clause("trans", "nx|!ny|z");
    M->add_clause("trans", "!z");

    M->add_clause("init", "!x|y");
    M->add_clause("init", "!nz");

    // Create IC3 instance
    IC3::IC3 * ic3_instance = new IC3::IC3(M);

    // Run the IC3 algorithm
//	ic3_instance->prove();

// Clean up
    delete ic3_instance;
    delete M;

    return 0;
}

// 2017-01-31
// TODO Generate SMT string from clauses
