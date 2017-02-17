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

#include <parser/parser.h>

#include <cctype>
#include <fstream>
#include <cassert>

namespace Parser {

/* Destructor for VMT_Driver object */
VMT_Driver::~VMT_Driver() {
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;

}

/* Function to parse VMT file from filename */
void VMT_Driver::parse(const char * const filename) {
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        exit( EXIT_FAILURE);
    }
    parse_helper(in_file);
    return;
}

/* Function to parse VMT file from stdin */
void VMT_Driver::parse(std::istream &stream) {
    if (!stream.good() && stream.eof()) {
        return;
    }
    //else
    parse_helper(stream);
    return;
}

/* Helper function to handle parsing of VMT */
void VMT_Driver::parse_helper(std::istream &stream) {

    delete (scanner);
    try {
        scanner = new VMT_Scanner(&stream);
    } catch (std::bad_alloc &ba) {
        std::cerr << "Failed to allocate scanner: (" << ba.what()
                << "), exiting!!\n";
        exit( EXIT_FAILURE);
    }

    delete (parser);
    try {
        parser = new VMT_Parser((*scanner) /* scanner */, (*this) /* driver */);
    } catch (std::bad_alloc &ba) {
        std::cerr << "Failed to allocate parser: (" << ba.what()
                << "), exiting!!\n";
        exit( EXIT_FAILURE);
    }
    const int accept(0);
    if (parser->parse() != accept) {
        std::cerr << "Parse failed!!\n";
    }
    return;
}

/* Create AST node for a symbol */
AST::ast_node *
VMT_Driver::mk_var(std::string symbol) {

    if (var_map.find(symbol) == var_map.end()) {
        AST::ast_node * node = new AST::ast_node;
        node->add_name(symbol);
        node->node_type(AST::SYM);
        var_map[symbol] = node;
        return node;
    } else {
        return var_map[symbol];
    }

}

/* Create AST node for an "OR" of multiple AST nodes */
AST::ast_node *
VMT_Driver::mk_or(std::vector<AST::ast_node *> operands) {

    AST::ast_node * node = new AST::ast_node;
    node->node_type(AST::OR);
    for (unsigned int i = 0; i < operands.size(); i++)
        node->add_operand(operands[i]);
    return node;
}

/* Create AST node for an "AND" of multiple AST nodes */
AST::ast_node *
VMT_Driver::mk_and(std::vector<AST::ast_node *> operands) {

    AST::ast_node * node = new AST::ast_node;
    node->node_type(AST::AND);
    for (unsigned int i = 0; i < operands.size(); i++)
        node->add_operand(operands[i]);
    return node;
}

/* Create AST node for a "NOT" of multiple AST nodes */
AST::ast_node *
VMT_Driver::mk_not(std::vector<AST::ast_node *> operands) {

    AST::ast_node * node = new AST::ast_node;
    node->node_type(AST::NOT);
    for (unsigned int i = 0; i < operands.size(); i++)
        node->add_operand(operands[i]);
    return node;
}


/* Create AST node for an "EQUALITY" of multiple AST nodes */
AST::ast_node *
VMT_Driver::mk_eq(std::vector<AST::ast_node *> operands) {

    AST::ast_node * node = new AST::ast_node;
    node->node_type(AST::EQ);
    for (unsigned int i = 0; i < operands.size(); i++)
        node->add_operand(operands[i]);
    return node;
}

/* Create AST node for a boolean value */
AST::ast_node *
VMT_Driver::mk_bool(bool value) {
    AST::ast_node * node = new AST::ast_node;
    node->node_type(AST::BOOL);
    node->add_val(value);
    return node;
}


AST::ast_node *
VMT_Driver::get_ast(std::string str) {
    return ast_map[str];
}

/* Add corresponding AST for each function definition appearing in the VMT file */
void VMT_Driver::add_definition(std::string str, AST::ast_node * a) {
    ast_map[str] = a;
}

/* Return map of symbols and their corresponding ASTs */
std::map<std::string, AST::ast_node *> VMT_Driver::get_var_map() {
    return var_map;
}

/* Return map of definitions and their corresponding ASTs */
std::map<std::string, AST::ast_node *> VMT_Driver::get_def_map() {
    return ast_map;
}

}
