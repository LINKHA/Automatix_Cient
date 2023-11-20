#include "actor.h"

namespace amx {

actor_manager::actor_manager() {}
actor_manager::~actor_manager() {}

bool actor_manager::init(int argc, char** argv) {
	[[maybe_unused]] auto host_init_guard = caf::detail::do_init_host_system(
		caf::detail::type_list<>{}, caf::detail::type_list<>{});
	caf::exec_main_init_meta_objects<>();
	caf::core::init_global_meta_objects();
	auto res = caf::exec_main<>([this](caf::actor_system& sys) {
		_actor_system = &sys;
		}, argc, argv);

	return res;
}

}
