
#ifndef PMW_DISPATCH_INTERNAL_H
#define PMW_DISPATCH_INTERNAL_H

#include <queue>
#include <string>
#include <vector>

#include <pthread.h>

#include <pmw_dispatch/dispatch.h>

//global variables for keeping state of the system
extern std::vector<dispatch_queue_t*> *queues;
extern pthread_mutex_t QUEUES_MUTEX;

struct dispatch_queue_t {

	int 						numThreads;	//number of concurrent threads in this queue
	dispatch_queue_type 		type;		//the type of queue
	std::string 				identifier;	//the string identifier for the queue
	std::queue<queue_function> 	queue;		//the queue of functions
	std::queue<void*> args;					//queue of arguments to functions
	std::vector<pthread_t>		threads;	//worker threads
	pthread_mutex_t 			queueMutex;	//mutex to get in the queue

};

//data to be passed to the worker thread -- we will pool them
struct WorkerThreadArgs;

//function to do work for a dispatch queue
void *dispatchWorkerThread(void *args);

void _addQueueToList(dispatch_queue_t *queue);

pthread_t _createWorkerThread();
void _destroyWorkerThread(pthread_t id);

dispatch_queue_t *_searchForQueue(const std::string &name);

#endif
