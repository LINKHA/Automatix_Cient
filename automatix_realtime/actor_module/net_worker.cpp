#include "net_worker.h"

#include <iostream>

namespace amx {
behavior net_worker::handler() {
	return {
		[&](msg_type::worker_handle params) {
			std::cout << params << std::endl;
		},
	};
}

}