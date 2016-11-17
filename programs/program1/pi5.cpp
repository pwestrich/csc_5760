
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <pthread.h>

#include "getRealTime.h"

//arguments for thread function
struct ThreadArgs {

	pthread_t		*myThread;	//my thread info
	pthread_mutex_t *sumMutex;	//mutex locking global sum
	uint64_t		start;		//my start location
	uint64_t		end;		//my end location
	double			*sum;		//pointer to shared sum

};

void* parallelPi(void *arg){

	//obtain arguemnts
	if (!arg){

		std::cerr << "Error: Null argument to thread function" << std::endl;
		exit(EXIT_FAILURE);

	}

	ThreadArgs *args = (ThreadArgs*)arg;

	if (!args){

		std::cerr << "Error: Bad argument to thread function" << std::endl;
		exit(EXIT_FAILURE);

	}

	//start doing my work
	double localSum = 0.0;
	double factor;

	if (args->start % 2){

		factor = -1.0; //negative on odd terms

	} else {

		factor = 1.0; //positive on even terms

	}

	for (size_t i = args->start; i < args->end; ++i, factor = -factor){

		localSum += (factor / (2 * i + 1));

	}

	pthread_mutex_lock(args->sumMutex);

	*(args->sum) += localSum;

	pthread_mutex_unlock(args->sumMutex);

	//exit, deleting arguments
	delete args;
	return NULL;

}

int main(const int argc, const char ** const argv){

	if (argc < 3){

		std:: cerr << "You must specify the number of threads and the accuracy of pi." << std::endl;
		return EXIT_FAILURE;

	}

	int threadCount = atoi(argv[1]);
	uint64_t accuracy = strtoull(argv[2], NULL, 10);

	double start = getRealTime();

	double sum = 0.0;
	uint64_t increment = (accuracy / threadCount) - 1;
	pthread_t *threads = new pthread_t[threadCount];
	pthread_mutex_t sumMutex = PTHREAD_MUTEX_INITIALIZER;

	//start threads
	for (int i = 0; i < threadCount; ++i){

		ThreadArgs *arg = new ThreadArgs;

		arg->myThread = &threads[i];
		arg->sumMutex = &sumMutex;
		arg->start 	  = i * increment;
		arg->end 	  = arg->start + increment;
		arg->sum 	  = &sum;

		pthread_create(&threads[i], NULL, parallelPi, static_cast<void*>(arg));

	}

	//join threads
	for (int i = 0; i < threadCount; ++i){

		pthread_join(threads[i], NULL);

	}

	sum = sum * 4.0;

	pthread_mutex_destroy(&sumMutex);
	delete [] threads;

	double end = getRealTime();
	double length = end - start;

	std::cout << "pi = " << std::setprecision(64) << sum << std::endl;
	std::cout << "took " << std::setprecision(64) << length << " seconds." << std::endl;

	return EXIT_SUCCESS;

}
