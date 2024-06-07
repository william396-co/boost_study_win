#include <iostream>
#include <stdexcept>
#include <boost/asio.hpp>

using boost::asio::ip::udp;


int main(int argc, char* argv[]) {

	if (argc != 3) {
		std::cerr << "Usage: client <host> <post>\n";
		return 1;
	}
	
	try {

		boost::asio::io_context io;
		udp::resolver resolver{io};		
		udp::endpoint receiver_endpoint =
			*resolver.resolve(udp::v4(), argv[1], argv[2]).begin();

		udp::socket socket(io);
		socket.open(udp::v4());

		std::array<char, 1> send_buf{{0}};
		socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

		std::array<char, 128> recv_buf;
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
		std::cout.write(recv_buf.data(), len);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}


	return 0;
}