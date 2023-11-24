#include <ylt/coro_rpc/coro_rpc_server.hpp>

inline std::string echo(std::string str) { return str; }

int main() {

	// 初始化服务器
	coro_rpc::coro_rpc_server server(/*thread_num =*/10, /*port =*/9000);

	server.register_handler<echo>(); // 注册rpc函数

	server.start(); // 启动server并阻塞等待
}