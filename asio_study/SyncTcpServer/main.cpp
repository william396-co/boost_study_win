#include <ctime>
#include <iostream>
#include <string>
#include <stdexcept>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;


std::string make_daytime_string() {
	time_t now = time(0);
	return ctime(&now);
}

int main() {
	try {

		boost::asio::io_context io;

		tcp::acceptor accepter(io, tcp::endpoint(tcp::v4(), 13));
		for (;;) {
			tcp::socket socket(io);
			accepter.accept(socket);
			std::string message = make_daytime_string();

			boost::system::error_code ignore_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignore_error);
		}

	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}
	return 0;
}