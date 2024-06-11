#include <iostream>
#include <memory>
#include <chrono>
#include <stdexcept>

#include <boost/asio.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/write.hpp>

using boost::asio::ip::tcp;


class session : public std::enable_shared_from_this<session>
{
public:
	explicit session(boost::asio::io_context& io, tcp::socket socket)
		:socket_{ std::move(socket) }
		, timer_{ io },
		strand_{ io.get_executor() } {
	}

	void go() {

		auto self(shared_from_this());
		boost::asio::spawn(strand_,
			[this, self](boost::asio::yield_context yield) {

				try {
					char data[128];
					while (true) {
						timer_.expires_after(std::chrono::seconds{10});
						std::size_t n = socket_.async_read_some(boost::asio::buffer(data), yield);
						boost::asio::async_write(socket_, boost::asio::buffer(data, n), yield);
					}
				}
				catch (std::exception& e) {
					socket_.close();
					timer_.cancel();
				}

			}, boost::asio::detached);

		boost::asio::spawn(strand_,
			[this, self](boost::asio::yield_context yield) {

				while (socket_.is_open()) {
					boost::system::error_code ignored_ec;
					timer_.async_wait(yield[ignored_ec]);
					if (timer_.expiry() <= boost::asio::steady_timer::clock_type::now()) {
						socket_.close();
					}
				}
			},
			boost::asio::detached);
	}
private:
	tcp::socket socket_;
	boost::asio::steady_timer timer_;
	boost::asio::strand<boost::asio::io_context::executor_type> strand_;
};


int main(int argc, char* argv[]) {

	try {

		if (argc != 2) {
			std::cerr << "Usage echo_server<port>\n";
			return 1;
		}

		boost::asio::io_context io;
		boost::asio::spawn(io,
			[&](boost::asio::yield_context yeild) {
				tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), std::atoi(argv[1])));

				while (true) {
					boost::system::error_code ec;
					tcp::socket socket(io);
					acceptor.async_accept(socket, yeild[ec]);
					if (!ec) {
						std::make_shared<session>(io, std::move(socket))->go();
					}
				}
			},
			[](std::exception_ptr e) {
				if (e) {
					std::rethrow_exception(e);
				}
			}
		);
		io.run();

	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}


	return 0;
}
