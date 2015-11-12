
#include <functional>
#include <string>
#include <vector>

#include <pthread.h>

#include "pmw_dispatch/dispatch.h"

struct dispatch_queue_t {



};

//initializes the dispatch environment
void dispatch_init(){



}

//uninitializes the dispatch environment
void disaptch_release(){



}

//accepts a function pointer to be added to the queue
bool dispatch_async_f(const dispatch_queue_t &queue, void (*work)(void *args), void *args){

	return false;

}

//accets a c++11 function object to be added to the queue
bool dispatch_async(const dispatch_queue_t &queue, std::function<void (void *args)> work, void *args){

	return false;

}

//gets a queue by name
dispatch_queue_t* dispatch_get_queue(const std::string &name){



}
