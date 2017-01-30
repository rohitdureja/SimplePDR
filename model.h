/*
 * Model.h
 *
 *  Created on: Jan 26, 2017
 *      Author: dureja
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <map>
#include "utils.h"

/*
 * ``Model'' class to maintain attributes of the transition system
 * Maintains the set of variables, the transition system (in CNF),
 * initial state (in CNF).
 */
class Model {
	/*
	 * Vector containing pointer to clauses clauses for the
	 * transition system.
	 */
	std::vector<Clause*> trans;
	std::vector<Clause*> init;

	// Helpers objects
	std::map<std::string, int8_t> var_map1; //	name -> int
	std::map<int8_t, std::string> var_map2; //	int -> name
	int8_t vcount;

public:

	Model() {
		vcount = 101;
	}
	~Model() {
		for(uint8_t i = 0 ; i < trans.size() ; ++i)
			delete trans[i];
		for(uint8_t i = 0 ; i < init.size() ; ++i)
			delete init[i];
	}
	void add_variable(const std::string);
	void add_clause(const std::string, const std::string);
	std::vector<Clause *> * get_trans();
	std::vector<Clause *> * get_init();
	void show_trans();
	void show_init();
};

#endif /* MODEL_H_ */
