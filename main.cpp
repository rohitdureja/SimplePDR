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
#include <memory>

int main() {
    // Specify transition system
    std::auto_ptr<Model::Model> M (new Model::Model());

    // state variables
    M->add_variable("a");
    M->add_variable("b");
    M->add_variable("na");
    M->add_variable("nb");

    // add current and next state variable relations
    M->add_variable_relation("a", "na");
    M->add_variable_relation("b", "nb");

    // initial states
    M->add_clause(Model::I, "!a");
    M->add_clause(Model::I, "!b");

    // transition relations
    M->add_clause(Model::T, "a||!b||nb");
    M->add_clause(Model::T, "a||b||!na");
    M->add_clause(Model::T, "!a||na");
    M->add_clause(Model::T, "!a||!nb");
    M->add_clause(Model::T, "b||!nb");

    // safety property
    M->add_clause(Model::P, "!a||b");


    // Create IC3 instance
    std::auto_ptr<IC3::IC3> ic3_instance(new IC3::IC3(M));

    // Run the IC3 algorithm
	ic3_instance->prove();

// Clean up
    delete ic3_instance;
    delete M;

    return 0;
}

// 2017-01-31
