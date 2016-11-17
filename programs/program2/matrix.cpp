
#include <cassert>
#include <cstdint>
#include <random>

#include <pthread.h>

#include "matrix.h"

struct Matrix {

	int64_t size;
	double *data;

};

struct MultiplyThreadArgs {

	int64_t startElement;
	int64_t endElement;

	Matrix *L;
	Matrix *R;
	Matrix *S;

};

//allocates a matrix
Matrix *createMatrix(const int64_t size){

	assert(size > 0);

	Matrix *mat = new Matrix;

	mat->size = size;
	mat->data = new double[size * size];

	return mat;

}

//deletes a matrix
void destroyMatrix(Matrix *&mat){

	if (mat){

		if (mat->data){

			delete [] mat->data;
			mat->data = NULL;

		}

		delete mat;
		mat = NULL;

	}

}

//initalizes a matrix
void initMatrix(Matrix * const mat){

	assert(mat);

	//fill the thing with lots of random numbers
	const uint64_t count = mat->size * mat->size;

	static std::default_random_engine generator;
  	static std::uniform_real_distribution<double> distribution(-1000000000.0, 1000000000.0);

	for (uint64_t i = 0; i < count; ++i){

		mat->data[i] = distribution(generator);

	}

}

//returns the element at the index given
double index(Matrix *const mat, const int64_t x, const int64_t y){

	assert(mat);

	const uint64_t i = x * mat->size + y;
	return mat->data[i];

}

void* multiplyThread(void *_args){

	MultiplyThreadArgs *args = (MultiplyThreadArgs*) _args;

	const int64_t size = args->S->size;
	const int64_t startCol = args->startElement / size;
	const int64_t startRow = args->startElement % size;
	const int64_t endCol = args->endElement / size;
	const int64_t endRow = args->endElement % size;
	
	for (int64_t i = startRow; i < endRow; ++i){

		//for every element in L's row
		for (int64_t j = startCol; j < endCol; ++j){

			double sum = 0.0;

			//for every element in R's column
			for (int64_t k = 0; k < size; ++k){

				//multiply them together and add to collective sum
				const double one = index(args->L, i, k);
				const double two = index(args->R, k, j);
				sum += one * two;

			}

			args->S->data[i * size + j] = sum;

		}

	}

	//clean up and leave
	delete args;
	return NULL;
	
}

void multiplyParallel(Matrix *const L, Matrix *const R, Matrix *const S, const int64_t numThreads){

	//first, determine the number of elements to calculate
	const int64_t numElements = S->size * S->size;

	//then determine how many per thread
	const int64_t numPerThread = numElements / numThreads;

	int64_t startElement = 0;

	pthread_t *threads = new pthread_t[numThreads - 1];

	for (int64_t i = 0; i < numThreads; ++i){

		//create and set arguments
		MultiplyThreadArgs *args = new MultiplyThreadArgs;

		args->L = L;
		args->R = R;
		args->S = S;
		args->startElement = startElement;

		startElement += numPerThread;

		args->endElement = startElement - 1;

		//run thread
		pthread_create(&threads[i], NULL, multiplyThread, (void*) args);

	}

	//join threads
	for (int64_t i = 0; i < numThreads; ++i){

		pthread_join(threads[i], NULL);

	}

	delete [] threads;
	threads = NULL;


}

void multiplySerial(Matrix *const L, Matrix *const R, Matrix *const S){

	assert(L && R && S);

	const int64_t size = S->size;

	//multiply elements
	for (int64_t i = 0; i < size; ++i){

		for (int64_t j = 0; j < size; ++j){

			double sum = 0.0;

			for (int64_t k = 0; k < size; ++k){

				const double one = index(L, i, k);
				const double two = index(R, k, j);
				sum += one * two;

			}

			S->data[i * size + j] = sum;

		}

	}

}
