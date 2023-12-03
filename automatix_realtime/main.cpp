#include "kernel/server.h"
#include "common/time.hpp"

using namespace amx;

int main(int argc, char** argv) {
	return_code::data res_code;

	time::timezone();

	auto try_operation = [&res_code](auto&& operation) {
		res_code = operation();
		return (res_code == return_code::success) ? res_code : res_code;
	};

	return try_operation([] { return server::get().print_logo(); })
		|| try_operation([] { return server::get().loadconfig(); })
		|| try_operation([&argc, &argv] { return server::get().launch(argc, argv); })
		|| try_operation([] { return server::get().exit(); });
}