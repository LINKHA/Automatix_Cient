#include "supervisor.h"
#include "concurrent/actor_config.h"

namespace amx {

behavior supervisor::handler() {
	return {
		[&](msg_type::add_monitor sub_actor) {
			auto act = g::act().get(sub_actor);
			this->monitor(act);
		},
		[&](msg_type::add_monitors sub_actors) {
			for (auto it : sub_actors) {
				auto act = g::act().get(it);
				this->monitor(act);
			}
		},
		[&](msg_type::add_monitor_by_id sub_actor) {
			auto act = g::act().get(sub_actor);
			this->monitor(act);
		},
		[&](msg_type::add_monitor_by_ids sub_actors) {
			for (auto it : sub_actors) {
				auto act = g::act().get(it);
				this->monitor(act);
			}
		},
	};
}

}
