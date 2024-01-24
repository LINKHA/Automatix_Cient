#include "tcp_client.h"
#include "asio.hpp"

using asio::ip::tcp;

namespace amx {
	tcp_client::tcp_client()
	{

	}

	tcp_client::~tcp_client()
	{

	}

	void tcp_client::run() {
		asio::io_context _io_context;
		net_client client(_io_context, "127.0.0.1", "10242");
		client.send_data("Hello, server!\n");
		_io_context.run();
	}
}
