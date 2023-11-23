#pragma once
#include "common/common.hpp"

#include <iostream>

namespace amx {
/// <summary>
/// Monitors the health status of child nodes. Usually, the supervisor does not perform logic
/// </summary>
class supervisor : public event_based_actor {
	DECLARE_CLASS(supervisor, event_based_actor)
public:
	/// <summary>
	/// root supervisor
	/// </summary>
	/// <param name="cfg"></param>
	supervisor(actor_config& cfg) : event_based_actor(cfg) {}
	/// <summary>
	/// substratum supervisor
	/// </summary>
	/// <param name="cfg"></param>
	/// <param name="base_actor"></param>
	supervisor(actor_config& cfg, vector<supervisor>& monitor_actors) 
		: event_based_actor(cfg) ,
		_monitor_actors(monitor_actors)
	{}

	behavior make_behavior() override {
		this->set_down_handler([this](const caf::down_msg& dm) {

		});
		for (auto it : _monitor_actors) {
			this->monitor(it);
		}
		
		return handler(); 
	}
private:
	unordered_map<string, std::function<void(int32_t)>> fun_map = {
		{"supervisor_handle", [this](int32_t b) { supervisor_handle(b); }}
	};
	behavior handler() {
		return {
			/*handler*/
			[&](const std::string& fun_name, int32_t b) { fun_map[fun_name](b); }
		};
	}

	void supervisor_handle(int32_t b) {
		std::cout << std::to_string(b) << std::endl;
	}

	vector<supervisor> _monitor_actors;
};

}