#include "actor_manager.h"
#include "automatix.h"

namespace amx {

bool actor_manager::launch(int argc, char** argv) {
	[[maybe_unused]] auto host_init_guard = caf::detail::do_init_host_system(
		caf::detail::type_list<>{}, caf::detail::type_list<>{});
	caf::exec_main_init_meta_objects<>();
	caf::core::init_global_meta_objects();
	auto res = caf::exec_main<>([this](caf::actor_system& sys) {
		_actor_system = &sys;

		automatix::get().launch_realtime();
		}, argc, argv);
	return res;
}

caf::actor_registry& actor_manager::registry() { return _actor_system->registry(); }

void actor_manager::launch_master_actor(){

}

}
