#pragma once
#include <memory>
#include "caf/all.hpp"

#include "base/module.h"

namespace amx {

class actor_manager;

//class master_actor : public caf::blocking_actor {
//public:
//
//};
//

class actor {
	friend class actor_manager;
public:
	explicit actor();
	virtual ~actor();

	void request() {}

	uint64_t id() { return _caf_actor.id(); }

	std::string name() { return ""; }
private:
	void set_caf_actor(caf::actor& acf_actor) {
		_caf_actor = acf_actor;
	}
	caf::actor _caf_actor;
};

}