
#include <iostream>
#include "Aplication.h"


int main() {
	Aplication app("tugevus-test");

	try {
		app.start();

	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		system("pause\n");
		return EXIT_FAILURE;
	}

	system("pause\n");
	return EXIT_SUCCESS;


}