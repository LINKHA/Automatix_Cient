#pragma once
#include <memory>
#include "common/common.hpp"

#include "caf/all.hpp"
#include "concurrent/actor.h"
#include "concurrent/master_actor.h"

namespace amx {

class actor_manager {
public:
	actor_manager(){}
	virtual ~actor_manager() {}
	bool launch(int argc, char** argv);

	template <typename... _Ty> uint64_t spawn(const string& name = "");

	const caf::actor& get(uint64_t id) { return registry().get<caf::actor>(id); }
	const caf::actor& get(const string& name) { return registry().get<caf::actor>(name); }

	caf::actor_registry& registry();
	
	caf::actor_system* _actor_system;

	void launch_master_actor();
	/*s_ptr<master_actor>*/
private:
	
};

template <typename... _Ty> uint64_t actor_manager::spawn(const string& name) {
	caf::actor act = _actor_system->spawn<_Ty...>();
	s_ptr<actor<_Ty...>> mact = make_shared<actor<_Ty...>>();
	mact->set_caf_actor(act);

	uint64_t id = mact->id();

	registry().put(id, act);
	registry().put(name, act);
	mact->_name = name;

	return id;
}

}