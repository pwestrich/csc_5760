
#include <cstdint>
#include <iostream>
#include <unistd.h>

#include <pmw_dispatch/dispatch.h>

#include "getRealTime.h"
#include "matrix.h"

int main(const int argc, const char **argv){

	if (argc < 3){

		std::cerr << "Error: too few arguments." << std::endl;
		return 1;

	}

	const int64_t SIZE 		  = std::stoull(argv[1]);
	const int64_t NUM_THREADS = std::stoull(argv[2]);

	if (SIZE < 10 || NUM_THREADS < 1){

		std::cerr << "Error: Invalid aruments " << SIZE << " and " << NUM_THREADS << std::endl;
		return 1;

	}

	dispatch_init();

	std::cout << "Creating matricies..." << std::endl;
	std::cout << "Size: " << SIZE << ", Threads: " << NUM_THREADS << std::endl;

	const double createStart = getRealTime();

	Matrix *mat1 = createMatrix(SIZE);
	Matrix *mat2 = createMatrix(SIZE);
	Matrix *mat3 = createMatrix(SIZE);

	initMatrix(mat1);
	initMatrix(mat2);

	const double createEnd = getRealTime();
	const double createElapsed = createEnd - createStart;

	std::cout << "Matricies created. Took " << createElapsed << " seconds." << std::endl;
	std::cout << "Multiplying..." << std::endl;

	const double multiplyStart = getRealTime();

	multiplyParallel(mat1, mat2, mat3, NUM_THREADS);

	const double multiplyEnd = getRealTime();
	const double multiplyElapsed = multiplyEnd - multiplyStart;
	
	std::cout << "Multiply finished. Took " << multiplyElapsed << " seconds." << std::endl;

	destroyMatrix(mat1);
	destroyMatrix(mat2);
	destroyMatrix(mat3);

	disaptch_release();
	return 0;

}
