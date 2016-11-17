
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "getRealTime.h"

#define BLOCK_SIZE 16

struct Matrix {

	float *data;
	int64_t size;
	int64_t numElements; 

};

__global__ void gpuMatrixMultiply(float *L, float *R, float *S, const int64_t size){

	//allocate some space
	float value = 0.0;

	//calculate my row and column
	const int64_t row = blockIdx.y * blockDim.y + threadIdx.y; 
	const int64_t col = blockIdx.x * blockDim.x + threadIdx.x; 

	//do not go over the array
	if (row >= size || col >= size) return;

	//sum up the terms
	for (int64_t i = 0; i < size; ++i){

		value += L[row * size + i] * R[i * size + col];

	}

	//set solution
	S[row * size + col] = value;

}

void matrixMultiply(const Matrix L, const Matrix R, const Matrix S){

	//first, allocate space on the GPU for each array
	float *gpuL = NULL;
	float *gpuR = NULL;
	float *gpuS = NULL;

	cudaError_t error = cudaMalloc(&gpuL, L.numElements * sizeof(float));
	if (error){

		printf("Error allocating matrix L on GPU: %s\n", cudaGetErrorString(error));
		exit(EXIT_FAILURE);

	}

	error = cudaMalloc(&gpuR, R.numElements * sizeof(float));
	if (error){

		printf("Error allocating matrix R on GPU: %s\n", cudaGetErrorString(error));
		exit(EXIT_FAILURE);

	}

	error = cudaMalloc(&gpuS, S.numElements * sizeof(float));
	if (error){

		printf("Error allocating matrix S on GPU: %s\n", cudaGetErrorString(error));
		exit(EXIT_FAILURE);

	}

	//now copy L and R
	error = cudaMemcpy(gpuL, L.data, L.numElements * sizeof(float), cudaMemcpyHostToDevice);
	if (error){

		printf("Error copying memory of L to GPU: %s\n", cudaGetErrorString(error));
		exit(EXIT_FAILURE);

	}

	error = cudaMemcpy(gpuR, R.data, R.numElements * sizeof(float), cudaMemcpyHostToDevice);
	if (error){

		printf("Error copying memory of R to GPU: %s\n", cudaGetErrorString(error));
		exit(EXIT_FAILURE);

	}

	//prepare to call GPU function
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid((S.size + dimBlock.x - 1) / dimBlock.x, (S.size + dimBlock.y) / dimBlock.y);

	gpuMatrixMultiply<<<dimGrid, dimBlock>>>(gpuL, gpuR, gpuS, S.size);
	
	error = cudaThreadSynchronize();
	printf("Multiply finished with status: %s\n", cudaGetErrorString(error));

	//copy S back
	error = cudaMemcpy(S.data, gpuS, S.numElements * sizeof(float), cudaMemcpyDeviceToHost);

	if (error){

		printf("Error copying memory of S from GPU: %s\n", cudaGetErrorString(error));
		exit(EXIT_FAILURE);

	}

	cudaFree(gpuL);
	cudaFree(gpuR);
	cudaFree(gpuS);

}

void usage(const char *name){

	printf("Usage: %s <size>\n", name);

}

int main(const int argc, const char **argv){

	if (argc < 2){

		usage(argv[0]);
		return EXIT_FAILURE;

	}

	//read the size of the matrix to make
	const int64_t size = atoll(argv[1]);
	const int64_t numElements = size * size;

	if (size < 0 || numElements < 0){

		printf("Error: Overflow in matrix size.\n");
		return EXIT_FAILURE;

	} else if (size < 1){

		printf("Error: Matrix size too small: %lli \n", size);

	}

	printf("Creating matricies...\n");

	const double startTime = getRealTime();

	Matrix L;
	L.size = size;
	L.numElements = numElements;
	L.data = (float*) malloc(numElements * sizeof(float));

	Matrix R;
	R.size = size;
	R.numElements = numElements;
	R.data = (float*) malloc(numElements * sizeof(float));

	Matrix S;
	S.size = size;
	S.numElements = numElements;
	S.data = (float*) malloc(numElements * sizeof(float));

	srand(NULL);

	for (int64_t i = 0; i < numElements; ++i){

		L.data[i] = rand() / 100.0;
		R.data[i] = rand() / 100.0;

	}

	const double initElapsed = getRealTime() - startTime;

	printf("Initilization complete. Tool %f seconds. \nMultiplying...\n", initElapsed);

	const double multiplyStart = getRealTime();

	matrixMultiply(L, R, S);

	const double multiplyElapsed = getRealTime() - multiplyStart;

	printf("Multiply complete. Took %f seconds.\n", multiplyElapsed);

	free(L.data);
	free(R.data);
	free(S.data);

	return 0;

}

