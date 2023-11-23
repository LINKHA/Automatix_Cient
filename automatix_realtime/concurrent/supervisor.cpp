#include "supervisor.h"

namespace amx {

void supervisor::supervisor_handle(const string& b) {
	std::cout << b << std::endl;
}

void supervisor::monitor_handle(const string& sub_actor) {
	auto act = g::act().get(sub_actor);
	this->monitor(act);
}

}
