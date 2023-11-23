#pragma once
#include "common/common.hpp"
#include "automatix.h"
#include "concurrent/actor_config.h"
namespace amx {
/// <summary>
/// Monitors the health status of child nodes. Usually, the supervisor does not perform logic
/// </summary>
class supervisor : public event_based_actor {
	DECLARE_CLASS(supervisor, event_based_actor)
public:
	supervisor(actor_config& cfg) : event_based_actor(cfg) {}

	behavior make_behavior() override {
		this->set_down_handler([this](const caf::down_msg& dm) {

		});
		return handler(); 
	}
private:
	behavior handler();
};

}