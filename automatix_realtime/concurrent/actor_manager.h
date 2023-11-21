#pragma once
#include <memory>
#include "caf/all.hpp"
#include "concurrent/actor.h"

namespace amx {

class actor_manager {
public:
	actor_manager();
	virtual ~actor_manager();
	bool init(int argc, char** argv);

	caf::actor_registry& registry();

	template <typename... _Ty> void spawn();

private:
	caf::actor_system* _actor_system;
};

template <typename... _Ty> void actor_manager::spawn() {
	caf::actor act = _actor_system->spawn<_Ty...>();
	std::shared_ptr<actor> mact = std::make_shared<actor>();
	mact->set_caf_actor(act);
	registry().put(mact->id(), act);
	registry().put(mact->name(), act);
}

}