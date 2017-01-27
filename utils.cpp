/*
 * utils.cpp
 *
 *  Created on: Jan 26, 2017
 *      Author: dureja
 */

#include "utils.h"

void split(const std::string &s, const char* delim, std::vector<std::string> & v){
    char * dup = strdup(s.c_str());
    char * token = std::strtok(dup, delim);
    while(token != NULL){
        v.push_back(std::string(token));
        token = std::strtok(NULL, delim);
    }
    free(dup);
}
