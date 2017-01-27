/*
 * utils.cpp
 *
 *  Created on: Jan 26, 2017
 *      Author: dureja
 */

#include "utils.h"


void split(const std::string &s, const char* delim, std::vector<std::string> & v){
    // to avoid modifying original string
    // first duplicate the original string and
	// return a char pointer then free the memory
    char * dup = strdup(s.c_str());
    char * token = std::strtok(dup, delim);
    while(token != NULL){
        v.push_back(std::string(token));
        // the call is treated as a subsequent calls to strtok:
        // the function continues from where it left in previous invocation
        token = std::strtok(NULL, delim);
    }
    free(dup);
}
