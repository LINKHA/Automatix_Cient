#pragma once
#include "common/common.hpp"
#include <iostream>
namespace amx {

class worker : public event_based_actor {
	DECLARE_CLASS(worker, event_based_actor)
public:
	worker(actor_config& cfg) : event_based_actor(cfg) {}
	behavior make_behavior() override { return handler(); }
private:
	unordered_map<string, std::function<void(int32_t)>> fun_map = {
		{"worker_handle", [this](int32_t b) { worker_handle(b); }}
	};
	behavior handler() {
		return {
			/*handler*/
			[&](const std::string& fun_name, int32_t b) { fun_map[fun_name](b); }
		};
	}

	void worker_handle(int32_t b) {
		std::cout << std::to_string(b) << std::endl;
	}
};

}