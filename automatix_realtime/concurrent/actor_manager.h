#pragma once
#include <memory>
#include "caf/all.hpp"
#include "concurrent/actor.h"

namespace amx {

class actor_manager {
public:
	actor_manager(){}
	virtual ~actor_manager() {}
	bool launch(int argc, char** argv);

	template <typename... _Ty> uint64_t spawn(const std::string& name = "");

	caf::actor_registry& registry();
	
	caf::actor_system* _actor_system;
private:
	
};

template <typename... _Ty> uint64_t actor_manager::spawn(const std::string& name) {
	caf::actor act = _actor_system->spawn<_Ty...>();
	std::shared_ptr<actor<_Ty...>> mact = std::make_shared<actor<_Ty...>>();
	mact->set_caf_actor(act);

	uint64_t id = mact->id();

	registry().put(id, act);
	registry().put(name, act);
	mact->_name = name;

	return id;
}

}