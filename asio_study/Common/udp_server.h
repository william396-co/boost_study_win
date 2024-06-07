#include <iostream>
#include <array>
#include <memory>
#include <functional>
#include <boost/asio.hpp>

#include "utils.h"

using boost::asio::ip::udp;

class udp_server {
public:
	udp_server(boost::asio::io_context& io, int16_t port) :
		socket_{ io,udp::endpoint(udp::v4(),port) } {
		start_receive();
	}	
private:

	void start_receive() {
		socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
			std::bind(&udp_server::handle_receive, this, std::placeholders::_1));
	}
	void handle_receive(boost::system::error_code const& error) {

		if (!error) {
			std::shared_ptr<std::string> message(new std::string(make_daytime_string()));
			
			socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
				std::bind(&udp_server::handle_send, this, message));
		}
		start_receive();
	}
	void handle_send(std::shared_ptr<std::string> const& msg) {

	}
private:
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	std::array<char, 1> recv_buffer_;
};