
#ifndef MATRIX_H
#define MATRIX_H

#include <cstdint>

struct Matrix;

//allocates a matrix
Matrix *createMatrix(const int64_t size);

//deletes a matrix
void destroyMatrix(Matrix *&mat);

//initalizes a matrix
void initMatrix(Matrix * const mat);

//returns the element at the index given
double index(Matrix *const mat, const int64_t x, const int64_t y);

void multiplyParallel(Matrix *const L, Matrix *const R, Matrix *const S, const int64_t numThreads);

void multiplySerial(Matrix *const L, Matrix *const R, Matrix *const S);

#endif
