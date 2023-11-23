#include "common/common.hpp"

namespace amx {

class net_actor : public event_based_actor {
public:
	net_actor(actor_config& cfg) : event_based_actor(cfg) {}
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