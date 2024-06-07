#include <iostream>
#include <stdexcept>
#include <array>
#include <string>
#include <ctime>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string() {
	time_t now = time(0);
	return ctime(&now);
}

class udp_server {
public:
	udp_server(boost::asio::io_context& io, int16_t port):
		socket_{io,udp::endpoint(udp::v4(),port)}
	{
		start_receive();
	}
private:
	void start_receive() {
		socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
			boost::bind(&udp_server::handle_receive, this, boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
	void handle_receive(boost::system::error_code const& error, size_t len) {
		if (!error) {
			std::shared_ptr<std::string> message(new std::string(make_daytime_string()));

			socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
				boost::bind(&udp_server::handle_send, this, message,
					boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred
				));
		}
		start_receive();
	}
	void handle_send(std::shared_ptr<std::string> message, boost::system::error_code const& error,
		size_t len) {		
	}
private:
	udp::socket socket_;
	udp::endpoint remote_endpoint_;
	std::array<char, 1> recv_buffer_;
};

int main(int argc, char* argv[]) {

	int16_t port = 9527;
	if (argc > 1) {
		port = std::strtol(argv[1], nullptr, 10);
	}

	try {
		boost::asio::io_context io;
		udp_server server{ io, port };
		io.run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}

	return 0;
}