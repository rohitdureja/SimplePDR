/*
 * Model.cpp
 *
 *  Created on: Jan 26, 2017
 *      Author: dureja
 */

#include "model.h"
#include "utils.h"

void Clause::add_literal(int8_t literal) {
	literals.push_back(literal);
}

int8_t Clause::get_literal() {
	return literals[0];
}

void Model::add_variable(const std::string var_name) {
	vars.push_back(var_name);
	var_map1[var_name] = vcount;
	var_map2[vcount] = var_name;
	vcount++;
}

int8_t Model::get_variable(const std::string var_name) {
	return var_map1[var_name];
}

void Model::add_clause(const std::string clause) {
	Clause *c = new Clause();
	std::vector<std::string> parse;
	split(clause, "&", parse);
	for (uint8_t i = 0 ; i < parse.size(); ++i) {
		std::cout << parse[i] << std::endl;
		std::cout << parse[i][0] << std::endl;
		if (parse[i][0] == '!') {
			parse[i].erase(parse[i].begin());
			c->add_literal(-var_map1[parse[i]]);
		}
		else
			c->add_literal(var_map1[parse[i]]);
	}
	ts.push_back(*c);
}

Clause * Model::get_clause() {
	return &ts[0];
}
