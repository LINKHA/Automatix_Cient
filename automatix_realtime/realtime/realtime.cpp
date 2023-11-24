#include "realtime.h"
#include "automatix.h"
#include <chrono>   
#include <thread>   

#include "actor_module/net_worker.h"

#include "concurrent/supervisor.h"

namespace amx {

void realtime::launch()
{
	//Creating a bridge_actor in realtime mode provides a realtime scoped call to the actor
	scoped_actor bridge_actor{ *g::act().actor_system_ };

	init(bridge_actor);
	tick(bridge_actor);
	exit(bridge_actor);
}

void realtime::init(const scoped_actor& bridge_actor)
{
	g::act().spawn<supervisor>("net_supervisor");
	g::act().spawn<net_worker>("net_worker1");

	auto supervisor = g::act().get("net_supervisor");
	bridge_actor->request(supervisor, std::chrono::seconds(10), msg_type::add_monitor("net_worker1"));
#if AMX_DEBUG
	test_.init();
#endif
}
void realtime::tick(const scoped_actor& bridge_actor)
{


	bool exit = false;


	while (!exit) {

		if (1) {

		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void realtime::exit(const scoped_actor& bridge_actor)
{
	
}

}