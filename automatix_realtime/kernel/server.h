#pragma once
#include "common/context.hpp"
#include "common/common.hpp"
#include "kernel/worker.h"

namespace amx {

	class server {
		DECLARE_CLASS(server, server)
	public:
		server() = default;

		~server();
		int init(int argc, char** argv, uint32_t worker_num);
		int run();
		void exit(int exitflag);

	private:
		context_manager context_manager_;

		volatile int exitflag_ = std::numeric_limits<int>::max();

		vector<std::unique_ptr<worker>> workers_;

#if AMX_DEBUG
		test::automatix_test test_;
#endif
	};

}