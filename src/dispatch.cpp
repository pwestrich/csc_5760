
#include <queue>
#include <string>
#include <vector>

#include <pthread.h>

#include <pmw_dispatch/dispatch.h>
#include "internal.h"

//initializes the dispatch environment
void dispatch_init(){

	//create the vector of queues
	queues = new std::vector<dispatch_queue_t*>();

	//make one default queue
	dispatch_create_queue(MAIN_QUEUE, QUEUE_SERIAL);

}

//uninitializes the dispatch environment
void disaptch_release(){

	//lock mutex for the queues
	pthread_mutex_lock(&QUEUES_MUTEX);

	for (size_t i = 0; i < queues->size(); ++i){

		dispatch_queue_t *queue = (*queues)[i];

		//lock queue's mutex
		pthread_mutex_lock(&queue->queueMutex);

		//halt every thread
		for (int i = 0; i < queue->numThreads; ++i){

			_destroyWorkerThread(queue->threads[i]);

		}

		//destroy mutex and queue
		pthread_mutex_destroy(&queue->queueMutex);
		sem_destroy(&queue->queueSem);
		delete queue;

	}

	delete queues;

}

//accepts a function pointer to be added to the queue
bool dispatch_async(dispatch_queue_t *queue, queue_function work, void *args){

	if (queue && work){

		pthread_mutex_lock(&queue->queueMutex);		//lock queue mutex
			queue->queue.push(work);				//push work and arguments
			queue->args.push(args);
		pthread_mutex_unlock(&queue->queueMutex);	//inlock mutex

		sem_post(&queue->queueSem);					//post semaphore to tell worker functions to work

		return true;

	} else return false;

}

//gets a queue by name
dispatch_queue_t* dispatch_get_queue(const std::string &name){

	return _searchForQueue(name);

}

dispatch_queue_t* dispatch_create_queue(const std::string &name, const dispatch_queue_type type, const int num){
 		
 	//no queues may ahve the same name
	if (_searchForQueue(name)) return NULL;

	dispatch_queue_t *newQ = new dispatch_queue_t;

		newQ->type = type;
		newQ->identifier = name;
		pthread_mutex_init(&newQ->queueMutex, NULL);
		sem_init(&newQ->queueSem, false, 0);

	if (type == QUEUE_SERIAL || num == 1){

		newQ->numThreads = 1;
		newQ->threads.push_back(_createWorkerThread(newQ));
		

	} else {

		newQ->numThreads = num;

		for (int i = 0; i < num; ++i){

			newQ->threads.push_back(_createWorkerThread(newQ));

		}

	}

	_addQueueToList(newQ);

	return newQ;

}

//destroys a queue by name
void dispatch_destroy_queue(const std::string &name){

	dispatch_queue_t *queue = _searchForQueue(name);

	if (queue){

		//lock queue's mutex
		pthread_mutex_lock(&queue->queueMutex);

		//halt every thread
		for (int i = 0; i < queue->numThreads; ++i){

			_destroyWorkerThread(queue->threads[i]);

		}

		//destroy mutex and queue
		pthread_mutex_destroy(&queue->queueMutex);
		sem_destroy(&queue->queueSem);
		delete queue;

	}

}
