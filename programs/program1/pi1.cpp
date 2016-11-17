
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <iomanip>

#include "getRealTime.h"

int main(const int argc, const char ** const argv){

	if (argc < 2){

		std::cerr << "You must specify the accurcy of pi." << std::endl;
		return EXIT_FAILURE;

	}

	uint64_t accuracy = strtoull(argv[1], NULL, 10);

	std::cout << "Running serial pi for " << accuracy << " loops..." << std::endl;

	double start = getRealTime();

	double factor = 1;
	double sum = 0;

	for (uint64_t i = 0; i < accuracy; i++, factor = -factor){

		sum += factor / (2 * i + 1);

	}

	sum = sum * 4.0;

	double end = getRealTime();
	double length = end - start;

	std::cout << "pi = " << std::setprecision(64) << sum << std::endl;
	std::cout << "took " << std::setprecision(64) << length << " seconds." << std::endl;

	return EXIT_SUCCESS;

}
