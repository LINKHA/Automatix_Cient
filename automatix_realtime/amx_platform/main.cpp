#include <amx_core/net/client.h>

namespace amx {
	int main() {
		try {
			asio::io_context io_context;
			tcp_client client(io_context, "127.0.0.1", "8080");
			client.send_data("Hello, server!\n");

			io_context.run();
		}
		catch (std::exception& e) {
			std::cerr << "Exception: " << e.what() << std::endl;
		}

		return 0;
	}
}
