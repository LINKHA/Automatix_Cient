#pragma once
#include "common/singleton.hpp"
#include "common/context.hpp"
#include "concurrent/actor_manager.h"
#include "realtime/realtime.h"

#include <memory>
#include <mutex>



namespace amx {

namespace return_code
{
	enum data
	{
		success,
		err
	};
};

class automatix
{
	REGISTER_SINGLETON(automatix)
public:
	return_code::data print_logo();
	return_code::data loadconfig();
	return_code::data launch(int argc, char** argv);
	return_code::data exit();

	void launch_realtime();

	context_manager _context_manager;
	actor_manager _actor_manager;
	realtime _rel_time;

	
private:
	std::once_flag _flag;
};

namespace g {
	static context_manager& ctx() {
		return automatix::get()._context_manager;
	}
	static actor_manager& act() {
		return automatix::get()._actor_manager;
	}
	static realtime& rt() {
		return automatix::get()._rel_time;
	}
}

}