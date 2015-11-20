
#include <iostream>
#include <vector>

#include "internal.h"

std::vector<dispatch_queue_t*> *queues;
pthread_mutex_t QUEUES_MUTEX = PTHREAD_MUTEX_INITIALIZER;

//data to be passed to the worker thread -- we will pool them
struct WorkerThreadArgs {


	pthread_mutex_t endMutex;
	bool 			shouldEnd;



};

//function to do work for a dispatch queue
void *dispatchWorkerThread(void *args){

	while (true){

		std::cout << "Idling..." << std::endl;
		sleep(-1);	//idle

	}

	return NULL;

}

void _addQueueToList(dispatch_queue_t *queue){

	pthread_mutex_lock(&QUEUES_MUTEX);

		queues->push_back(queue);

	pthread_mutex_unlock(&QUEUES_MUTEX);

}

pthread_t _createWorkerThread(){

	WorkerThreadArgs *args = new WorkerThreadArgs;

	pthread_t thread;
	int rc = pthread_create(&thread, NULL, dispatchWorkerThread, args);

	if (rc < 0){

		std::cerr << "ERROR: Could not create thread." << std::endl;
		exit(EXIT_FAILURE);

	}

	return thread;

}

void _destroyWorkerThread(pthread_t id){

	pthread_cancel(id);

}

dispatch_queue_t *_searchForQueue(const std::string &name){

	//do not allow modifications to the queue list while we do this
	pthread_mutex_lock(&QUEUES_MUTEX);

	for (size_t i = 0; i < queues->size(); ++i){

		if ((*queues)[i]->identifier == name){

			pthread_mutex_unlock(&QUEUES_MUTEX);
			return (*queues)[i];

		}

	}

	//queue not found, return NULL, unlock mutex
	pthread_mutex_unlock(&QUEUES_MUTEX);
	return NULL;

}
