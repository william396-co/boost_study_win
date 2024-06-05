#include <array>
#include <iostream>
#include <stdexcept>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	try {
		if (argc != 2) {
			std::cerr << "Usage Client<Host>\n";
			return 1;
		}

		boost::asio::io_context io;
		tcp::resolver resolver(io);
		tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");

		tcp::socket socket(io);
		boost::asio::connect(socket, endpoints);

		for (;;) {
			std::array<char, 128> buf;
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			if (error == boost::asio::error::eof)
				break;
			else if (error) {
				throw boost::system::system_error(error);
			}
			std::cout.write(buf.data(), len);
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}

	return 0;
}