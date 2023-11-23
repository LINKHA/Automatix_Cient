#pragma once
#include "common/common.hpp"
#include "concurrent/worker.h"

#include <iostream>

namespace amx {

class net_worker : public worker {
public:
	net_worker(actor_config& cfg) : worker(cfg) {}
	behavior make_behavior() override { return handler(); }
private:
	unordered_map<string, std::function<void(int32_t)>> fun_map = {
		{"net_handle", [this](int32_t b) { net_handle(b); }}
	};
	behavior handler() {
		return {
			/*handler*/
			[&](const std::string& fun_name, int32_t b) { fun_map[fun_name](b); }
		};
	}

	void net_handle(int32_t b) {
		std::cout << std::to_string(b) << std::endl;
	}
};

}