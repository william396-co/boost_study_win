#include <iostream>
#include <stdexcept>
#include <string>
#include <ctime>
#include <boost/asio.hpp>

using boost::asio::ip::udp;


std::string make_daytime_string() {
	time_t now = time(0);
	return ctime(&now);
}

int main(int argc, char* argv[]) {

	auto port = 9527;
	if (argc > 1) {
		port = std::strtol(argv[1], nullptr, 10);
	}

	try {

		boost::asio::io_context io;
		udp::socket socket(io, udp::endpoint(udp::v4(), port));

		while (true)
		{
			std::array<char, 1> recv_buf;
			udp::endpoint remote_endpoint;
			socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
			std::string message = make_daytime_string();
			boost::system::error_code ignore_error;
			socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, ignore_error);
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}

	return 0;
}