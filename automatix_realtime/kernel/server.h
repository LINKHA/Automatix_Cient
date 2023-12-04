#pragma once
#include "common/singleton.hpp"
#include "common/context.hpp"
#include "common/return_code.hpp"

#include <memory>
#include <mutex>



namespace amx {

class server
{
public:
	server() = default;

	~server();
	int init(int argc, char** argv, uint32_t worker_num);
	int run();
	void exit(int exitflag);

private:
	context_manager context_manager_;

	volatile int exitflag_ = std::numeric_limits<int>::max();
#if AMX_DEBUG
	test::automatix_test test_;
#endif
};

}