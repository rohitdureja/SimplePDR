/*
 * Solver.h
 *
 *  Created on: Jan 30, 2017
 *      Author: dureja
 */

#ifndef SOLVER_H_
#define SOLVER_H_

namespace Solver {

class Solver {
public:
	Solver();
	virtual ~Solver();
	void push();
	void pop();
	void add_var();
	void add_constraints();
	void check_sat();
};

} /* namespace Solver */

#endif /* SOLVER_H_ */
