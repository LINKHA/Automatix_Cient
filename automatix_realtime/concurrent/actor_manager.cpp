#include "actor_manager.h"
#include "automatix.h"

#include "concurrent/actor_config.h"

namespace amx {

actor_manager::actor_manager() {
}

actor_manager::~actor_manager() {
}

bool actor_manager::launch(int argc, char** argv) {
	[[maybe_unused]] auto host_init_guard = caf::detail::do_init_host_system(
		caf::detail::type_list<>{}, caf::detail::type_list<caf::id_block::amx_msg_type>{});
	caf::exec_main_init_meta_objects<caf::id_block::amx_msg_type>();
	caf::core::init_global_meta_objects();
	auto res = caf::exec_main<caf::id_block::amx_msg_type>([this](caf::actor_system& sys) {
		actor_system_ = &sys;
		automatix::get().launch_realtime();
		}, argc, argv);
	return res;
}

caf::actor_registry& actor_manager::registry() { return actor_system_->registry(); }

}
