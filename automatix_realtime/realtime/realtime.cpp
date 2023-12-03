#include "realtime.h"
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
	test_.init();
#endif
}
void realtime::tick()
{
	bool exit = false;

	while (!exit) {

		if (1) {

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void realtime::exit()
{
	
}

}