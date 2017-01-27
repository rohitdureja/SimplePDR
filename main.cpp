#include "model.h"
#include "utils.h"

int main() {
	Clause *c;
	Model *M = new Model();
	M->add_variable("x");
	M->add_variable("y");
	M->add_variable("z");
	M->add_clause("!x&z");
	c = M->get_clause();
	std::cout << int(c->get_literal()) << std::endl;
	delete M;
}
