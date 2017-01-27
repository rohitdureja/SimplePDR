/*
 * Model.cpp
 *
 *  Created on: Jan 26, 2017
 *      Author: dureja
 */

#include "model.h"
#include "utils.h"

/**
 *
 */
void Clause::add_literal(int8_t literal) {
	literals.push_back(literal);
}

std::vector<int8_t> * Clause::get_literals() {
	return &literals;
}

void Model::add_variable(const std::string var_name) {
	var_map1[var_name] = vcount;
	var_map2[vcount] = var_name;
	vcount++;
}

void Model::add_clause(const std::string type, const std::string clause) {
	Clause *c = new Clause();
	std::vector<std::string> parse;
	split(clause, "|", parse);
	for (uint8_t i = 0 ; i < parse.size(); ++i) {
		if (parse[i][0] == '!') {
			parse[i].erase(parse[i].begin());
			c->add_literal(-var_map1[parse[i]]);
		}
		else
			c->add_literal(var_map1[parse[i]]);
	}
	if (type == "trans")
		trans.push_back(c);
	else if (type == "init")
		init.push_back(c);
	else
		std::cout << "Undefined type!" << std::endl;
}

std::vector<Clause*> * Model::get_trans() {
	return &trans;
}
std::vector<Clause*> * Model::get_init() {
	return &init;
}

void Model::show_trans() {
	std::vector<int8_t> * c;
	for (uint8_t i = 0 ; i < trans.size() ; ++i) {
			c = trans[i]->get_literals();
			for (uint8_t j = 0 ; j < c->size() ; ++j)
				std::cout << ((*c)[j] > 0 ? var_map2[(*c)[j]] : "!" + var_map2[-(*c)[j]]) << std::endl;
		}
}

void Model::show_init() {
	std::vector<int8_t> * c;
	for (uint8_t i = 0 ; i < init.size() ; ++i) {
			c = init[i]->get_literals();
			for (uint8_t j = 0 ; j < c->size() ; ++j)
				std::cout << ((*c)[j] > 0 ? var_map2[(*c)[j]] : "!" + var_map2[-(*c)[j]]) << std::endl;
		}
}
