#pragma once
#include "common/common.hpp"
#include "concurrent/actor_config.h"

namespace amx {

class worker : public event_based_actor {
	DECLARE_CLASS(worker, event_based_actor)
public:
	worker(actor_config& cfg) : event_based_actor(cfg) {}
	behavior make_behavior() override { return handler(); }
private:
	behavior handler();
};

}