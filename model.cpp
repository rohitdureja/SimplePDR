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
#include "utils.h"

void Model::add_variable(const std::string var_name) {
	var_map1[var_name] = vcount;
	str_vars.push_back(var_name);
	var_map2[vcount] = var_name;
	vcount = vcount + 2;
}

void Model::add_clause(const std::string type, const std::string clause) {
	Clause *c = new Clause();
	std::vector<std::string> parse;
	split(clause, "|", parse);
	for (unsigned char i = 0 ; i < parse.size(); ++i) {
		if (parse[i][0] == '!') {
			parse[i].erase(parse[i].begin());
			c->add_literal(var_map1[parse[i]]+1);
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
	for (unsigned char i = 0 ; i < trans.size() ; ++i) {
			c = trans[i]->get_literals();
			for (unsigned char j = 0 ; j < c->size() ; ++j)
				std::cout << ((*c)[j] % 2 == 0 ? var_map2[(*c)[j]] : "!" + var_map2[(*c)[j]-1]) + " ";
			std::cout << std::endl;
		}
}

void Model::show_init() {
	std::vector<signed char> * c;
	for (unsigned char i = 0 ; i < init.size() ; ++i) {
			c = init[i]->get_literals();
			for (unsigned char j = 0 ; j < c->size() ; ++j)
				std::cout << ((*c)[j] % 2 == 0 ? var_map2[(*c)[j]] : "!" + var_map2[(*c)[j]-1]) + " ";
			std::cout << std::endl;
		}
}

std::vector<std::string> * Model::get_variables() {
	return &str_vars;
}

std::vector<unsigned char> * Model::get_encoded_variables() {
	return &enc_vars;
}
