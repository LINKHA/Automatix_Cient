#pragma once
#include "common/singleton.hpp"
#include "common/context.hpp"
#include "realtime/realtime.h"
#include "common/return_code.hpp"

#include <memory>
#include <mutex>



namespace amx {

class server
{
	REGISTER_SINGLETON(server)
public:
	return_code::data print_logo();
	return_code::data loadconfig();
	return_code::data launch(int argc, char** argv);
	return_code::data exit();

	void launch_realtime();

	context_manager context_manager_;
	realtime rel_time_;

	
private:
	std::once_flag _flag;
};

namespace g {
	static context_manager& ctx() {
		return server::get().context_manager_;
	}
	static realtime& rt() {
		return server::get().rel_time_;
	}
}

}