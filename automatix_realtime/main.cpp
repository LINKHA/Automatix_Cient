#include "automatix.h"

using namespace amx;

int main(int argc, char** argv) {
	return_code::data res_code;

	auto try_operation = [&res_code](auto&& operation) {
		res_code = operation();
		return (res_code == return_code::success) ? res_code : res_code;
	};

	return try_operation([] { return automatix::get().loadconfig(); })
		|| try_operation([&argc, &argv] { return automatix::get().launch(argc, argv); })
		|| try_operation([] { return automatix::get().tick(); })
		|| try_operation([] { return automatix::get().exit(); });
}