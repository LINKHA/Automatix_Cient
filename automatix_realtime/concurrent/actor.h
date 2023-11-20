#pragma once
#include <memory>
#include "caf/actor_ostream.hpp"
#include "caf/actor_system.hpp"
#include "caf/caf_main.hpp"
#include "caf/event_based_actor.hpp"

#include "base/module.h"

namespace amx {

class actor_manager {
public:
	actor_manager();
	virtual ~actor_manager();
	bool init(int argc, char** argv);


private:
	caf::actor_system* _actor_system;
};

}