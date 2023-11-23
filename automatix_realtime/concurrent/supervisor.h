#pragma once
#include "common/common.hpp"
#include "automatix.h"

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

	behavior make_behavior() override {
		this->set_down_handler([this](const caf::down_msg& dm) {

		});
		
		return handler(); 
	}
private:
	unordered_map<string, std::function<void(const string& sub_actor)>> fun_map = {
		{"supervisor_handle", [this](const string& b) { supervisor_handle(b); }},
		{"monitor_handle", [this](const string& sub_actor) { monitor_handle(sub_actor); }}
	};
	behavior handler() {
		return {
			/*handler*/
			[&](const std::string& fun_name, const string& b) { fun_map[fun_name](b); }
		};
	}

	void supervisor_handle(const string&  b);

	void monitor_handle(const string& sub_actor);
};

}