/*
 * utils.h
 *
 *  Created on: Jan 26, 2017
 *      Author: dureja
 */

#ifndef UTILS_H_
#define UTILS_H_
#include <cstring>
#include <string>
#include <vector>

void split(const std::string &s, const char* delim, std::vector<std::string> & v);

/*
 * Clause class to maintain a vector of encoded literals.
 * Note: Negative values in the vector denote a negated literal.
 */
class Clause {
	/*
	 * Vector containing signed values for literals
	 */
	std::vector<int8_t> literals;
public:
	/*
	 * Member function to add a literal to a clause
	 * \param t The value to write
	 */
	void add_literal(int8_t);
	/*
	 * Member function to return a pointer to the vector containing
	 * literals in the clause.
	 * \return a pointer to the literals vectors
	 */
	std::vector<int8_t> * get_literals();
};

#endif /* UTILS_H_ */
