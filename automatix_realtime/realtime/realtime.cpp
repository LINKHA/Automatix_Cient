#include "realtime.h"
#include "automatix.h"
#include <chrono>   
#include <thread>   

namespace amx {

void realtime::launch()
{
	init();
	tick();
	exit();
}

void realtime::init()
{
#if AMX_DEBUG
	_test.init();
#endif
}
void realtime::tick()
{
	bool exit = false;
	while (!exit) {


		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void realtime::exit()
{
	
}

}