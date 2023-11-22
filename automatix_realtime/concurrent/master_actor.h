#pragma once
#include "common/common.hpp"

#include "concurrent/actor.h"

namespace amx {

//void handler(const string& fun_name, int32_t b) {
//	// Your logic for addition
//}
//
//void exit_handler(blocking_actor* self, bool& running, caf::exit_msg& em) {
//	if (em.reason) {
//		self->fail_state(std::move(em.reason));
//		running = false;
//	}
//}
//
//void blocking_calculator_fun(blocking_actor* self) {
//	bool running = true;
//	self->receive_while(running)( //
//		handler,
//		[&](caf::exit_msg& em) { exit_handler(self, running, em); }
//	);
//}
//
//class master_actor : public blocking_actor {
//public:
//	master_actor(actor_config& cfg) : blocking_actor(cfg) {}
//	void act() override { blocking_calculator_fun(this); }
//};

}