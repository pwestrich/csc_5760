
#ifndef PMW_DISPATCH_INTERNAL_H
#define PMW_DISPATCH_INTERNAL_H

#include <queue>
#include <string>
#include <vector>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include <pmw_dispatch/dispatch.h>

//global variables for keeping state of the system
extern std::vector<dispatch_queue_t*> *queues;
extern pthread_mutex_t QUEUES_MUTEX;

struct queue_item {

	queue_function  work;			//function to be executed
	void*			args;			//arguments to function
	sem_t			sem_complete;	//semaphore to indicate completion (if needed)
	bool			should_post;	//should the worker thread post the semaphore?

};

struct dispatch_queue_t {

	int 						numThreads;	//number of concurrent threads in this queue
	dispatch_queue_type 		type;		//the type of queue
	std::string 				identifier;	//the string identifier for the queue
	std::queue<queue_item*>	 	queue;		//the queue of functions
	std::vector<pthread_t>		threads;	//worker threads
	pthread_mutex_t 			queueMutex;	//mutex to get in the queue
	sem_t						queueSem;	//semaphore to indicate to the worker threads that there is work to do

};

//data to be passed to the worker thread -- we will pool them
struct WorkerThreadArgs;

//function to do work for a dispatch queue
void *dispatchWorkerThread(void *args);

void _addQueueToList(dispatch_queue_t *queue);

pthread_t _createWorkerThread(dispatch_queue_t *queue);
void _destroyWorkerThread(pthread_t id);

dispatch_queue_t *_searchForQueue(const std::string &name);

#endif
