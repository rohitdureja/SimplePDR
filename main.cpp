#include "model.h"
#include "IC3.h"

int main() {
	// Specify transition system
	Model *M = new Model();
	M->add_variable("x");
	M->add_variable("y");
	M->add_variable("z");
	M->add_clause("trans", "!x|z");
	M->add_clause("trans", "!y|!z");
	M->add_clause("trans", "x|!y");
	M->add_clause("init", "!x|y");
	M->add_clause("init", "z");

	// Create IC3 instance
	IC3::IC3 * ic3_instance = new IC3::IC3(M);

	// Run the IC3 algorithm
	ic3_instance->prove();

	// Clean up
	delete M;
	delete ic3_instance;

	return 0;
}
