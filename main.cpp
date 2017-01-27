#include "model.h"
#include "utils.h"

int main() {
	Model *M = new Model();
	M->add_variable("x");
	M->add_variable("y");
	M->add_variable("z");
	M->add_clause("trans", "!x|z");
	M->add_clause("trans", "!y|!z");
	M->add_clause("trans", "x|!y");
	M->show_trans();
	delete M;
}
