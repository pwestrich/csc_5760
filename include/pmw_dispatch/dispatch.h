
#ifndef PMW_DISPATCH_H
#define PMW_DISPATCH_H

#include <string>

const std::string MAIN_QUEUE = "main";

typedef void* (*queue_function)(void *args);
enum dispatch_queue_type : uint8_t {QUEUE_SERIAL, QUEUE_CONCORRENT};

struct dispatch_queue_t;

//initializes the dispatch environment
void dispatch_init();

//uninitializes the dispatch environment
void disaptch_release();

//accepts a function pointer to be added to the queue
bool dispatch_async(dispatch_queue_t *queue, queue_function work, void *args);

//gets a queue by name
dispatch_queue_t* dispatch_get_queue(const std::string &name);

//creates a queue and returns a reference to it
dispatch_queue_t* dispatch_create_queue(const std::string &name, const dispatch_queue_type type = QUEUE_SERIAL, const int num = 0);

//destroys a queue by name
void dispatch_destroy_queue(const std::string &name);

#endif
