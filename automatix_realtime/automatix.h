#pragma once
#include "common/singleton.hpp"
#include "common/context.hpp"
#include "concurrent/actor_manager.h"
#include "realtime/realtime.h"
#include "common/return_code.hpp"

#include <memory>
#include <mutex>



namespace amx {

class automatix
{
	REGISTER_SINGLETON(automatix)
public:
	return_code::data print_logo();
	return_code::data loadconfig();
	return_code::data launch(int argc, char** argv);
	return_code::data exit();

	void launch_realtime();

	context_manager context_manager_;
	actor_manager actor_manager_;
	realtime rel_time_;

	
private:
	std::once_flag _flag;
};

namespace g {
	static context_manager& ctx() {
		return automatix::get().context_manager_;
	}
	static actor_manager& act() {
		return automatix::get().actor_manager_;
	}
	static realtime& rt() {
		return automatix::get().rel_time_;
	}
}

}