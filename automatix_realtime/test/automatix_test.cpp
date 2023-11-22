#include "automatix_test.h"
#include "automatix.h"

#include "caf/all.hpp"

#include <iostream>
#include <cassert>

namespace amx {
namespace test {
/***************************************************************/
//actor test
namespace t1 {
caf::behavior calculator_fun(caf::event_based_actor* self) {
	return {
			[=](const std::string& what) -> std::string {
			std::cout << what << std::endl;
			return "calculator_fun1";
			},
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

caf::behavior calculator_fun2(caf::event_based_actor* self) {
	
	auto a1 = g::act().registry().get<caf::actor>("act1");

	self->request(a1, std::chrono::seconds(100), "Hello World!")
		.then(
			// ... wait up to 10s for a response ...
			[=](const std::string& what) {
				// ... and print it
				std::cout << what << std::endl;
			});

	return {
			[=](const std::string& what) -> std::string {
			std::cout << what << std::endl;
			return "";
			},
	};
}

class calculator2 : public caf::event_based_actor {
public:
	calculator2(caf::actor_config& cfg) : caf::event_based_actor(cfg) {
		// nop
	}

	caf::behavior make_behavior() override {
		return calculator_fun2(this);
	}
};

void test() {
	uint64_t id1 = g::act().spawn<calculator>("act1");
	uint64_t id2 = g::act().spawn<calculator2>("act2");

	auto a1 = g::act().registry().get<caf::actor>(id1);
	auto a2 = g::act().registry().get<caf::actor>("act1");

	assert(a1 == a2);
}

}

/***************************************************************/


void automatix_test::init() {
	t1::test();
}

void automatix_test::tick() {

}

}
}