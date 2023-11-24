#include "automatix_test.h"
#include "automatix.h"
#include "common/common.hpp"


#include "caf/all.hpp"

#include <iostream>
#include <cassert>

#include "concurrent/worker.h"
#include "concurrent/supervisor.h"

#include "common/var_list.hpp"
#include "concurrent/actor_config.h"

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
//actor test
namespace t2 {
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

void test() {	
	uint64_t id1 = g::act().spawn<calculator>("act1");
	auto a1 = g::act().registry().get<caf::actor>(id1);

	caf::scoped_actor self{ *g::act().actor_system_ };
	self->send(a1, "hi there");
	self->request(a1, std::chrono::seconds(10), "hi there2");
}

}
/***************************************************************/
//blocking_actor test
namespace t3 {
class master_actor : public blocking_actor {
public:
	master_actor(actor_config& cfg) : blocking_actor(cfg) {}
	void act() override { handler(); }
private:
	unordered_map<string, std::function<void(int32_t)>> fun_map = {
		{"func1", [this](int32_t b) { func1(b); }},
		{"func2", [this](int32_t b) { func2(b); }}
	};
	void handler() {
		bool running = true;
		this->receive_while(running)( 
			/*handler*/
			[&](const std::string& fun_name, int32_t b) { fun_map[fun_name](b); },
			/*exit_handler*/
			[&](caf::exit_msg& em) { 
				if (em.reason) {
					this->fail_state(std::move(em.reason));
					running = false;
				}
			}
		);
	}

	void func1(int32_t b) {
		auto a1 = g::act().get("act1");
		this->request(a1, std::chrono::seconds(10), "func2", 2);
		std::cout << std::to_string(b) << std::endl;
	}

	void func2(int32_t b) {
		auto a2 = g::act().get("act2");
		this->request(a2, std::chrono::seconds(10), "func2", 2);
		std::cout << std::to_string(b) << std::endl;
	}
};

class based_actor : public event_based_actor {
public:
	based_actor(actor_config& cfg) : event_based_actor(cfg) {}
	behavior make_behavior() override { return handler(); }
private:
	unordered_map<string, std::function<void(int32_t)>> fun_map = {
		{"func1", [this](int32_t b) { func1(b); }},
		{"func2", [this](int32_t b) { func2(b); }}
	};
	behavior handler() {
		return {
			/*handler*/
			[&](const std::string& fun_name, int32_t b) { fun_map[fun_name](b); }
		};
	}

	void func1(int32_t b) {
		auto a3 = g::act().get("act3");
		this->request(a3, std::chrono::seconds(10), "func2", 1);
		std::cout << std::to_string(b) << std::endl;
	}

	void func2(int32_t b) {
		auto a2 = g::act().get("act2");
		this->request(a2, std::chrono::seconds(10), "func1", 2);
		std::cout << std::to_string(b) << std::endl;
	}

	void func3(int32_t b) {
		auto a4 = g::act().get("act4");
		this->request(a4, std::chrono::seconds(10), "func3", 2);
		std::cout << std::to_string(b) << std::endl;
	}
};

void test() {
	uint64_t id1 = g::act().spawn<master_actor>("act1");
	uint64_t id2 = g::act().spawn<based_actor>("act2");
	uint64_t id3 = g::act().spawn<based_actor>("act3");
	//4Ñ­»·
	uint64_t id4 = g::act().spawn<based_actor>("act4");

	auto a1 = g::act().get(id1);
	auto a2 = g::act().get(id2);

	caf::scoped_actor self{ *g::act().actor_system_ };
	self->request(a1, std::chrono::seconds(10), "func1", 2);
	self->request(a2, std::chrono::seconds(10), "func1", 2);
}

}
/***************************************************************/
//blocking_actor test
namespace t4 {
	class master_actor : public blocking_actor {
	public:
		master_actor(actor_config& cfg) : blocking_actor(cfg) {}
		void act() override { handler(); }
	private:
		unordered_map<string, std::function<void(int32_t)>> fun_map = {
			{"master_tick", [this](int32_t b) { master_tick(b); }}
		};
		void handler() {
			bool running = true;
			this->receive_while(running)(
				/*handler*/
				[&](const std::string& fun_name, int32_t b) { fun_map[fun_name](b); },
				/*exit_handler*/
				[&](caf::exit_msg& em) {
					if (em.reason) {
						this->fail_state(std::move(em.reason));
						running = false;
					}
				}
			);
		}

		void master_tick(int32_t b) {
			auto a2 = g::act().get("act2");
			auto a3 = g::act().get("act3");
			auto a4 = g::act().get("act4");

			bool exit = false;
			/*while (!exit) {*/
				std::cout << "master_tick" << std::endl;
				this->request(a2, std::chrono::seconds(10), "func1", 2);
				this->request(a3, std::chrono::seconds(10), "func1", 3);
				this->request(a4, std::chrono::seconds(10), "func1", 4);
			//}
		}
	};

	class based_actor : public event_based_actor {
	public:
		based_actor(actor_config& cfg) : event_based_actor(cfg) {}
		behavior make_behavior() override { return handler(); }
	private:
		unordered_map<string, std::function<void(int32_t)>> fun_map = {
			{"func1", [this](int32_t b) { func1(b); }}
		};
		behavior handler() {
			return {
				/*handler*/
				[&](const std::string& fun_name, int32_t b) { fun_map[fun_name](b); }
			};
		}

		void func1(int32_t b) {
			std::cout << std::to_string(b) << std::endl;
		}
	};

	void test() {
		uint64_t id1 = g::act().spawn<master_actor>("act1");
		g::act().spawn<based_actor>("act2");
		g::act().spawn<based_actor>("act3");
		g::act().spawn<based_actor>("act4");

		auto a1 = g::act().get(id1);

		caf::scoped_actor self{ *g::act().actor_system_ };
		self->request(a1, std::chrono::seconds(10), "master_tick", 2);
	}
}
/***************************************************************/
//supervisor
namespace t5 {

class based_actor : public worker {
public:
	based_actor(actor_config& cfg) : worker(cfg) {}
	behavior make_behavior() override { return handler(); }
private:
	unordered_map<string, std::function<void(int32_t)>> fun_map = {
		{"func1", [this](int32_t b) { func1(b); }}
	};
	behavior handler() {
		return {
			/*handler*/
			[&](const std::string& fun_name, int32_t b) { fun_map[fun_name](b); }
		};
	}

	void func1(int32_t b) {
		std::cout << std::to_string(b) << std::endl;
	}
};

void test() {
	g::act().spawn<supervisor>("net_supervisor");
	g::act().spawn<worker>("worker1");
	g::act().spawn<worker>("worker2");
	g::act().spawn<worker>("worker3");
	g::act().spawn<worker>("worker4");
	auto supervisor = g::act().get("net_supervisor");

	caf::scoped_actor self{ *g::act().actor_system_ };
	self->request(supervisor, std::chrono::seconds(10), msg_type::add_monitor("worker1"));
	self->request(supervisor, std::chrono::seconds(10), msg_type::add_monitors{ "worker2", "worker3", "worker4" });
}

}
/***************************************************************/

void automatix_test::init() {
	//t1::test();
	//t2::test();
	//t3::test();
	//t4::test();
	t5::test();
}

void automatix_test::tick() {

}

}
}