
#include <iostream>

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

	for (int i = 0; i < 10; ++i){

		int *args =new int;
		*args = i;
		dispatch_async(dispatch_get_queue(MAIN_QUEUE), test, static_cast<void*>(args));

	}

	sleep(1);
	std::cout << "Waiting..." << std::endl;
	sleep(10);
	disaptch_release();
	return 0;

}
