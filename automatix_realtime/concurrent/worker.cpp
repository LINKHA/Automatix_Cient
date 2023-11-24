#include "worker.h"
#include <iostream>
namespace amx {

behavior worker::handler() {
	return {
		[&](msg_type::worker_handle params) {
			std::cout << params << std::endl;
		},
	};
}

}