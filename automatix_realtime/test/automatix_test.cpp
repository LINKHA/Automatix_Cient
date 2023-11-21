#include "automatix_test.h"
#include "automatix.h"

#include "caf/all.hpp"

namespace amx {

caf::behavior calculator_fun(caf::event_based_actor*) {
	return {
		[](int32_t a, int32_t b) { return a + b; },
		[](int32_t a, int32_t b) { return a - b; },
	};
}

class calculator : public caf::event_based_actor {
public:
	calculator(caf::actor_config& cfg) : caf::event_based_actor(cfg) {
		// nop
	}

	caf::behavior make_behavior() override {
		return calculator_fun(this);
	}
};

void automatix_test::init() {
	automatix::get()._actor_manager.spawn<calculator>();
}

void automatix_test::tick() {

}

}