#include "realtime.h"
#include "automatix.h"
#include <chrono>   
#include <thread>   

#include "actor_module/net_actor.h"

namespace amx {

void realtime::launch()
{
	//Creating a bridge_actor in realtime mode provides a realtime scoped call to the actor
	scoped_actor bridge_actor{ *g::act()._actor_system };

	init(bridge_actor);
	tick(bridge_actor);
	exit(bridge_actor);
}

void realtime::init(const scoped_actor& bridge_actor)
{
	g::act().spawn<net_actor>("net_actor");


#if AMX_DEBUG
	_test.init();
#endif
}
void realtime::tick(const scoped_actor& bridge_actor)
{
	bool exit = false;
	auto net_actor = g::act().get("net_actor");

	while (!exit) {

		if (1) {
			bridge_actor->request(net_actor, std::chrono::seconds(10), "net_handle", 2);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void realtime::exit(const scoped_actor& bridge_actor)
{
	
}

}