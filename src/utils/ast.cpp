#include <utils/ast.h>

namespace AST {
/* destructor for AST node */
ast_node::~ast_node() {
    for (unsigned int i = 0; i < operands.size(); ++i)
        delete operands[i];
    id.clear();
}

/* add operands to AST node */
void ast_node::add_operand(ast_node * ast) {
    operands.push_back(ast);
}

/* configure type of node */
void ast_node::node_type(otype t) {
    op = t;
}

/* add name if node is of type SYM */
void ast_node::add_name(std::string str) {
    id = str;
}

/* add value if node is of type BOOL */
void ast_node::add_val(bool v) {
    val = v;
}

/* get operands of the AST node */
std::vector<ast_node *> ast_node::get_operands() {
    return operands;
}

/* get the type of AST node */
otype ast_node::get_ast_type() {
    return op;
}

/* get the name if node is of type SYM */
std::string ast_node::get_ast_string() {
    return id;
}

/* get the value if the node is of type BOOL */
bool ast_node::get_ast_val() {
    return val;
}

}
