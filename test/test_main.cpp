
#include <iostream>

#include <pmw_dispatch/dispatch.h>

int main(const int argc, const char **argv){

	dispatch_init();

	sleep(10);
	std::cout << "test" << std::endl;

	disaptch_release();

	return 0;

}
