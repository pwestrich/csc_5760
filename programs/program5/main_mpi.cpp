/*
*	main_mpi.cpp
*	
*	by Philip Westrich
*	for CSC 4760 Assignment #5
*	"Game of Life: MPI Edition"
*
*	This program uses MPI to solve the Game of Life problem on a distributed system.
*	Note that I am using the C++ libraries, since this is C++, after all.
*/

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <random>
//#include <signal.h>

#include <mpi.h>

#include "getRealTime.h"

using namespace std;

const int MASTER = 0;	//the master shall be rank 0

inline void usage(const char *name){

	cerr << "Usage: ./" << name << " <size> <generations>" << endl;

}

int main(int argc, char **argv){

	//initialize the MPI environment and find my rank in the world
	MPI::Init(argc, argv);
	const int rank = MPI::COMM_WORLD.Get_rank();
	const int size = MPI::COMM_WORLD.Get_size();

	//cout << "Rank :" << rank << ", PID: " << getpid() << endl;
	//raise(SIGSTOP);

	//read the parameters of the problem from the command line
	//MPI::Init apparently removes the extra MPI args from the arguments
	if (argc < 3){

		cerr << "Error: Too few arguments." << endl;
		usage(argv[0]);

		//close the MPI environment and return
		MPI::Finalize();
		return EXIT_FAILURE;

	}

	const int64_t problemSize = stoll(argv[1]);
	const int64_t generations = stoll(argv[2]);

	if (problemSize < 3){

		cerr << "Error: Size too small: " << size << endl;
		usage(argv[0]);

		//close the MPI environment and return
		MPI::Finalize();
		return EXIT_FAILURE;

	} else if (generations < 2){

		cerr << "Error: Too few generations: " << generations << endl;
		usage(argv[0]);

		//close the MPI environment and return
		MPI::Finalize();
		return EXIT_FAILURE;

	}

	if (problemSize % size){

		cerr << "Error: Not an even distribution. Please rerun with the problem size evenly divisible\
				 by the number of proccesses." << endl;
		MPI::Finalize();
		return EXIT_FAILURE;

	}

	//Each proccess will keep its own partition of the problem size and initialize it.
	const int64_t myPartition = problemSize / size;
	const int64_t myPartitionSize = myPartition * myPartition;

	//only print this once
	if (rank == MASTER){

		cout << "Starting with " << size << " proccesses and problem size " << problemSize << endl;

	}

	const double startTime = getRealTime();

	char *currentGeneration = new char[myPartitionSize];
	char *nextGeneration    = new char[myPartitionSize];
	char *aboveRow 			= new char[myPartition];
	char *belowRow			= new char[myPartition];

	const int aboveRank = rank ? (rank - 1) : (size - 1);
	const int belowRank = (rank + 1) % size;

	//initialize the first grid
	default_random_engine generator;
	uniform_int_distribution<char> distribution(0, 1);

	for (int64_t i = 0; i < myPartitionSize; ++i){

		currentGeneration[i] = distribution(generator);

	}

	const double initTime = getRealTime();

	//now for each generation, do work
	for(int64_t i = 0; i < generations; ++i){

		//first, get the above and below rows from the above and below ranks
		//this will also act as a barrier until the other two proccesses get here each generation
		const char *currentRowStart = currentGeneration + (i * myPartition);

		MPI::COMM_WORLD.Sendrecv(currentRowStart, myPartition, MPI_CHAR, aboveRank, 0, 
								 aboveRow, myPartition, MPI_CHAR, belowRank, 0);

		//then for every element in my grid
		for (int64_t j = 0; j < myPartition; ++j){

			for (int64_t k = 0; k < myPartition; ++k){

				//calculate the values of my adjacent cells
				const int64_t leftCol  = (k == 0) ? (myPartition - 1) : (k - 1);
				const int64_t rightCol = (k == myPartition - 1) ? 0 : (k + 1);
				const int64_t topRow   = j - 1;
				const int64_t botRow   = j + 1;

				//and the current one
				const int64_t index = j * myPartition + k;

				//then the sum of the cells adjacent
				char sum = currentGeneration[j * myPartition + leftCol];
				sum	    += currentGeneration[j * myPartition + rightCol];

				if (topRow < 0){

					//use the seperate top row for calculations
					sum += aboveRow[leftCol];
					sum += aboveRow[k];
					sum	+= aboveRow[rightCol];

				} else {

					//do it normally
					sum += currentGeneration[topRow * myPartition + leftCol];
					sum += currentGeneration[topRow * myPartition + k];
					sum	+= currentGeneration[topRow * myPartition + rightCol];

				}

				if (botRow >= myPartition){

					//use seperate bottom row
					sum += belowRow[leftCol];
					sum += belowRow[k];
					sum += belowRow[rightCol];

				} else {

					//do it normally
					sum += currentGeneration[botRow * myPartition + leftCol];
					sum += currentGeneration[botRow * myPartition + k];
					sum += currentGeneration[botRow * myPartition + rightCol];

				}

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

	if (rank == MASTER){

		const double now = getRealTime();
		const double totalElapsedTime = now - startTime;
		const double initElapsedTime = initTime - startTime;
		const double probElapsedTime = now - initTime;

		cout << "Finished. Took " << totalElapsedTime << " seconds in total, "
			 << initElapsedTime << " seconds initalizing, and "
			 << probElapsedTime << " seconds solving." << endl;

	}

	//close the MPI environment and return
	MPI::Finalize();
	return EXIT_SUCCESS;

}
