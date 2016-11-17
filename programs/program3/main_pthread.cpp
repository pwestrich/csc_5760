
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <string>

#include "getRealTime.h"
#include "matrix.h"

void usage(const char *name){

	std::cerr << "Usage: ./" << name << " <size> <num_threads>" << std::endl; 

}

int main(const int argc, const char **argv){

	if (argc < 3){

		usage(argv[0]);
		exit(EXIT_FAILURE);

	}

	//read the size of the matrix to make
	const int64_t size = std::stoll(argv[1]);
	const int64_t numThreads = std::stoll(argv[2]);

	if (size <= 0){

		std::cerr << "Error: Overflow in matrix size." << std::endl;
		exit(EXIT_FAILURE);

	} else if (numThreads <= 0){

		std::cerr << "Error: Overflow in thread count." << std::endl;
		exit(EXIT_FAILURE);

	}

	std::cout << "Creating matricies..." << std::endl;
	std::cout << "Size: " << size << ", Threads: " << numThreads << std::endl;

	const double createStart = getRealTime();

	Matrix *mat1 = createMatrix(size);
	Matrix *mat2 = createMatrix(size);
	Matrix *mat3 = createMatrix(size);

	initMatrix(mat1);
	initMatrix(mat2);

	const double createEnd = getRealTime();
	const double createElapsed = createEnd - createStart;

	std::cout << "Matricies created. Took " << createElapsed << " seconds." << std::endl;
	std::cout << "Multiplying with pthreads..." << std::endl;

	const double multiplyStart = getRealTime();

	multiplyPthread(mat1, mat2, mat3, numThreads);

	const double multiplyEnd = getRealTime();
	const double multiplyElapsed = multiplyEnd - multiplyStart;
	
	std::cout << "Multiply finished. Took " << multiplyElapsed << " seconds." << std::endl;

	destroyMatrix(mat1);
	destroyMatrix(mat2);
	destroyMatrix(mat3);

	return 0;

}
