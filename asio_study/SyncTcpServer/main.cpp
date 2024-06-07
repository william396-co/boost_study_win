#include <ctime>
#include <iostream>
#include <string>
#include <stdexcept>

#include <boost/asio.hpp>
#include "../Common/utils.h"

using boost::asio::ip::tcp;


int main(int argc,char* argv[]) {
	auto port = 9527;
	if (argc > 1) {
		port = std::strtol(argv[1], nullptr, 10);
	}

	try {

		boost::asio::io_context io;

		tcp::acceptor accepter(io, tcp::endpoint(tcp::v4(), port));
		for (;;) {
			tcp::socket socket(io);
			accepter.accept(socket);
			std::string message = utils::make_daytime_string();

			boost::system::error_code ignore_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignore_error);
		}

	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}
	return 0;
}