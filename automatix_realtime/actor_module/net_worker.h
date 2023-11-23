#pragma once
#include "common/common.hpp"
#include "concurrent/worker.h"

#include <iostream>

namespace amx {

class net_worker : public worker {
public:
	net_worker(actor_config& cfg) : worker(cfg) {}
	behavior make_behavior() override { return handler(); }
private:
	behavior handler();
};

}