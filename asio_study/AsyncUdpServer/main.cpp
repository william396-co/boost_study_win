#include <iostream>
#include <stdexcept>
#include <boost/asio.hpp>

#include "../Common/udp_server.h"

int main(int argc, char* argv[]) {

	auto port = 9527;
	if (argc > 1) {		
		port = std::atoi(argv[1]);
	}

	try {
		boost::asio::io_context io;
		udp_server server(io, port);
		io.run();
	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}

	return 0;
}