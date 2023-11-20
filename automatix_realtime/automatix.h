#pragma once
#include "common/singleton.hpp"
#include "common/context.hpp"
#include "concurrent/actor.h"

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
	return_code::data loadconfig();
	return_code::data launch(int argc, char** argv);
	return_code::data tick();
	return_code::data exit();

	context_manager _context_manager;
	actor_manager _actor_manager;
};

#define CONTEXT automatix::get()._context_manager
#define CONTEXT_BACKGROUND automatix::get()._context_manager.get_context(0)

}