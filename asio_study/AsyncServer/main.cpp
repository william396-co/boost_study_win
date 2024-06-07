#include <iostream>
#include <stdexcept>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "../Common/udp_server.h"


int main(int argc, char* argv[]) {

	int16_t port = 9527;
	if (argc > 1) {
		port = std::strtol(argv[1], nullptr, 10);
	}
	try {

		boost::asio::io_context io;
		udp_server server1{ io,port };

		io.run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}

	return 0;
}