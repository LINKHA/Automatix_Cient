#pragma once
#include "common/singleton.hpp"
#include "common/context.hpp"
#include "concurrent/actor_manager.h"

#include <memory>

#define AMX_DEBUG 1

#if AMX_DEBUG
#include "test/automatix_test.h"
#endif

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
	return_code::data tick();
	return_code::data exit();

	context_manager _context_manager;
	actor_manager _actor_manager;

#if AMX_DEBUG
	automatix_test _test;
#endif

};

#define CONTEXT automatix::get()._context_manager
#define CONTEXT_BACKGROUND automatix::get()._context_manager.get_context(0)
#define ACTOR automatix::get()._actor_manager
}