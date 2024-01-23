#include <iostream>

#include "asio.hpp"

namespace amx {

	class tcp_client {
	public:
		tcp_client(asio::io_context& io_context, const std::string& server_address, const std::string& server_port)
			: io_context_(io_context),
			resolver_(io_context),
			socket_(io_context) {
			asio::ip::tcp::resolver::results_type endpoints = resolver_.resolve(server_address, server_port);

			asio::async_connect(socket_, endpoints,
				[this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
					if (!ec) {
						std::cout << "Connected to server: " << endpoint << std::endl;
						start_reading();
					}
					else {
						std::cerr << "Error connecting to server: " << ec.message() << std::endl;
					}
				});
		}

		void send_data(const std::string& data) {
			asio::async_write(socket_, asio::buffer(data),
				[this](std::error_code ec, std::size_t /*length*/) {
					if (ec) {
						std::cerr << "Error sending data: " << ec.message() << std::endl;
					}
				});
		}

		void close() {
			io_context_.post([this]() { socket_.close(); });
		}

	private:
		void start_reading() {
			asio::async_read_until(socket_, asio::dynamic_buffer(receive_buffer_), '\n',
				[this](std::error_code ec, std::size_t /*length*/) {
					if (!ec) {
						std::cout << "Received data: " << receive_buffer_ << std::endl;
						receive_buffer_.clear();
						start_reading();
					}
					else {
						std::cerr << "Error receiving data: " << ec.message() << std::endl;
						close();
					}
				});
		}

	private:
		asio::io_context& io_context_;
		asio::ip::tcp::resolver resolver_;
		asio::ip::tcp::socket socket_;
		std::string receive_buffer_;
	};

}
