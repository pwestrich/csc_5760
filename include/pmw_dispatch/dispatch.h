
#ifndef PMW_DISPATCH_H
#define PMW_DISPATCH_H

#include <functional>
#include <string>

struct dispatch_queue_t;

//initializes the dispatch environment
void dispatch_init();

//uninitializes the dispatch environment
void disaptch_release();

//accepts a function pointer to be added to the queue
bool dispatch_async_f(const dispatch_queue_t &queue, void (*work)(void *args), void *args);

//accets a c++11 function object to be added to the queue
bool dispatch_async(const dispatch_queue_t &queue, std::function<void (void *args)> work, void *args);

//gets a queue by name
dispatch_queue_t* dispatch_get_queue(const std::string &name);

#endif
