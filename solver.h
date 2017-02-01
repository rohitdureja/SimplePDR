/*
 * Solver.h
 *
 *  Created on: Jan 30, 2017
 *      Author: dureja
 */

#ifndef SOLVER_H_
#define SOLVER_H_
#include "z3++.h"

namespace Solver {

enum result {sat, unsat, unknown};
enum type {Integer, Boolean};

class Solver {
private:
	z3::context * c;
	z3::solver * s;
	int x;
public:
	Solver();
	virtual ~Solver();
	void push();
	void pop();
	void add_symbol(type);
	void add_assertion();
	bool check_sat();

};

} /* namespace Solver */

#endif /* SOLVER_H_ */
