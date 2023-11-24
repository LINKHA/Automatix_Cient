#include <ylt/coro_rpc/coro_rpc_server.hpp>

inline std::string echo(std::string str) { return str; }

int main() {

	// ��ʼ��������
	coro_rpc::coro_rpc_server server(/*thread_num =*/10, /*port =*/9000);

	server.register_handler<echo>(); // ע��rpc����

	server.start(); // ����server�������ȴ�
}