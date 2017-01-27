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

class Clause {
	std::vector<int8_t> literals;
public:
	void add_literal(int8_t);
	int8_t get_literal();
};

class Model {
	// Transition system
	std::vector<std::string> vars;
	std::vector<Clause> ts;

	// Helpers objects
	std::map<std::string, int8_t> var_map1; //	name -> int
	std::map<int8_t, std::string> var_map2; //	int -> name
	int8_t vcount;

public:
	Model() {
		vcount = 101;
	}
	void add_variable(const std::string);
	int8_t get_variable(const std::string);
	void add_clause(const std::string);
	Clause * get_clause();
};

#endif /* MODEL_H_ */
