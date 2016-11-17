
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>

#include "getRealTime.h"

using namespace std;

inline void usage(const char *name){

	cout << "Usage: " << name << " <problem_size> <generations>" << endl;

}

void printGrid(ostream &out, char *grid, const int64_t gridSize){

	out << "Grid ---------------------------" << endl;

	for (int64_t i = 0; i < gridSize; ++i){

		for (int64_t j = 0; j < gridSize; ++j){

			out << static_cast<int>(grid[i * gridSize + j]) << " ";

		}

		out << endl;

	}

}

int main(const int argc, const char **argv){

	//check argument count
	if (argc < 3){

		usage(argv[0]);
		return 1;

	}

	//determine problem size and number of genreations
	const int64_t gridSize = stoll(argv[1]);
	const int64_t generations = stoll(argv[2]);
	const int64_t numElements = gridSize * gridSize;

	if (gridSize < 3 || numElements < 0){

		cerr << "Error: Grid size " << gridSize << " is too small." << endl;
		return 1;

	} else if (generations < 2){

		cerr << "Number of generations " << generations << " is too small." << endl;
		return 1;

	}

	cout << "Building start grid..." << endl;
	cout << "Size: " << gridSize << endl;

	const double buildStart = getRealTime();

	//build the two arrays that will be the current grid and next grid
	char *currentGeneration = new char[numElements];
	char *nextGeneration = new char[numElements];

	//initialize the first grid
	default_random_engine generator;
	uniform_int_distribution<char> distribution(0, 1);

	for (int64_t i = 0; i < numElements; ++i){

		currentGeneration[i] = distribution(generator);

	}

	const double buildTime = getRealTime() - buildStart;

	cout << "Build finished. Took " << buildTime << " seconds." << endl;
	cout << "Simulating..." << endl;

	const double start = getRealTime();

	//start calculating the generations
	for (int64_t i = 0; i < generations; ++i){

		//cout << "Generation " << i << endl;
		//printGrid(cout, currentGeneration, gridSize);

		for (int64_t j = 0; j < gridSize; ++j){

			for (int64_t k = 0; k < gridSize; ++k){

				//calculate this element's neighbors, looping over the edge
				const int64_t leftCol  = k == 0 ? gridSize - 1 : k - 1;
				const int64_t rightCol = k == gridSize - 1 ? 0 : k + 1;
				const int64_t topRow   = j == 0 ? gridSize - 1 : j - 1;
				const int64_t botRow   = j == gridSize - 1 ? 0 : j + 1;

				//sum up the neighbors
				char sum  = currentGeneration[topRow * gridSize + leftCol];
				sum 	+= currentGeneration[topRow * gridSize + k];
				sum		+= currentGeneration[topRow * gridSize + rightCol];
				sum 	+= currentGeneration[j * gridSize + leftCol];
				sum		+= currentGeneration[j * gridSize + rightCol];
				sum 	+= currentGeneration[botRow * gridSize + leftCol];
				sum 	+= currentGeneration[botRow * gridSize + k];
				sum 	+= currentGeneration[botRow * gridSize + rightCol];

				const int64_t index = j * gridSize + k;

				//if he is alive
				if (currentGeneration[index] == 1){

					if (sum < 2 || sum > 3){

						//dead from starvation or overcrowding
						nextGeneration[index] = 0;

					} else {

						//lives to see another day
						nextGeneration[index] = 1;

					}

				} else if (sum == 3){

					//he is reborn
					nextGeneration[index] = 1;

				} else {

					//dead by default
					nextGeneration[index] = 0;

				}

			}

		}

		//swap generations
		swap(currentGeneration, nextGeneration);

	}

	const double elapsedTime = getRealTime() - start;

	cout << "Done. Took " << elapsedTime << " seconds." << endl;

	delete [] currentGeneration;
	delete [] nextGeneration;

	return 0;

}
