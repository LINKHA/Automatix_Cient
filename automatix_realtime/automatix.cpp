#include "automatix.h"

namespace amx {

IMPLEMENT_SINGLETON(automatix)
	

return_code::data automatix::loadconfig()
{
	return return_code::err;
}

return_code::data automatix::launch(int argc, char** argv)
{
	// create id 0 context
	std::shared_ptr<context> background = _context_manager.create_context();
	background->with_value("argc", argc);
	background->with_value("argv", argv);

	_actor_manager.init(argc, argv);
	return return_code::success;
}

return_code::data automatix::tick()
{
	return return_code::success;
}

return_code::data automatix::exit()
{
	return return_code::success;
}

}