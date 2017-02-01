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
	s = new Solver::Solver();
}

bool IC3::prove() {
	return true;
}

IC3::~IC3() {
	delete s;
}

} /* namespace IC3 */
