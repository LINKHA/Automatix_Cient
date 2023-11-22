#pragma once
#include <memory>
#include "common/common.hpp"
#include "caf/all.hpp"

#include "base/module.h"

namespace amx {

class actor_manager;

template <typename _Ty>
class actor {
	friend class actor_manager;
public:
	explicit actor(){}
	virtual ~actor() {}

	void request_then(const actor& consumer) {}
	void request_then(uint64_t actor_id) {}
	void request_then(const string& name) {}

	void request_sync(const actor& consumer) {}
	void request_sync(uint64_t actor_id) {}
	void request_sync(const string& name) {}
	//
	void send_then(const actor& consumer) {}
	void send_then(uint64_t actor_id) {}
	void send_then(const string& name) {}
	//����ֱͣ�����еȴ���responses������Ұ��պ���ȳ���˳����д���
	void send_sync(const actor& consumer) {}
	void send_sync(uint64_t actor_id) {}
	void send_sync(const string& name) {}

	uint64_t id() { return _caf_actor.id(); }
	string name() { return _name; }
private:
	void set_caf_actor(caf::actor& acf_actor) {
		_caf_actor = acf_actor;
	}
	caf::actor _caf_actor;
	string _name{};
};

}