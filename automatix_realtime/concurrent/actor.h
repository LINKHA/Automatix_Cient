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
template <typename _Ty>
class actor {
	friend class actor_manager;
public:
	explicit actor(){}
	virtual ~actor() {}

	void request_then(const actor& consumer) {}
	void request_then(uint64_t actor_id) {}
	void request_then(const std::string& name) {}

	void request_sync(const actor& consumer) {}
	void request_sync(uint64_t actor_id) {}
	void request_sync(const std::string& name) {}
	//
	void send_then(const actor& consumer) {}
	void send_then(uint64_t actor_id) {}
	void send_then(const std::string& name) {}
	//会暂停直到所有等待的responses到达，并且按照后进先出的顺序进行处理
	void send_sync(const actor& consumer) {}
	void send_sync(uint64_t actor_id) {}
	void send_sync(const std::string& name) {}


	uint64_t id() { return _caf_actor.id(); }
	std::string name() { return _name; }
private:
	void set_caf_actor(caf::actor& acf_actor) {
		_caf_actor = acf_actor;
	}
	caf::actor _caf_actor;
	std::string _name{};
};

}