
#include <iostream>
#include <unistd.h>

#include <pmw_dispatch/dispatch.h>

void* test(void *args){

	int *num = reinterpret_cast<int*>(args);

	std::cout << "thread #" << *num << std::endl;

	sleep(1);

	delete num;
	return NULL;

}

int main(const int argc, const char **argv){

	dispatch_init();

	//test the main queue
	for (int i = 0; i < 10; ++i){

		int *args = new int;
		*args = i;
		dispatch_async(dispatch_get_queue(MAIN_QUEUE), test, static_cast<void*>(args));

	}

	sleep(1);
	std::cout << "Waiting..." << std::endl;
	sleep(10);

	//now try making a new queue
	dispatch_queue_t *newQueue = dispatch_create_queue("testQ", QUEUE_SERIAL);

	//test the new queue
	for (int i = 10; i < 20; ++i){

		int *args = new int;
		*args = i;
		dispatch_async(newQueue, test, static_cast<void*>(args));

	}

	sleep(1);
	std::cout << "Waiting again......" << std::endl;
	sleep(10);

	//try making a concurrent queue
	dispatch_queue_t *newQueue2 = dispatch_create_queue("testQ", QUEUE_CONCORRENT, 2);

	//test the concurrent queue
	for (int i = 20; i < 30; ++i){

		int *args = new int;
		*args = i;
		dispatch_async(newQueue2, test, static_cast<void*>(args));

	}

	sleep(1);
	std::cout << "Waiting take three..." << std::endl;
	sleep(10);

	disaptch_release();
	return 0;

}
