/*
 * IC3.h
 *
 *  Created on: Jan 30, 2017
 *      Author: dureja
 */

#ifndef IC3_H_
#define IC3_H_

#include "model.h"
#include "solver.h"

namespace IC3 {

class IC3 {
private:
	Solver::Solver * s;
public:
	IC3(Model *);
	bool prove();
	virtual ~IC3();
};

} /* namespace IC3 */

#endif /* IC3_H_ */
