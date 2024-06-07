#include <iostream>
#include <memory>

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

			});
	}
private:
	tcp::socket socket_;
	boost::asio::steady_timer timer_;
	boost::asio::strand<boost::asio::io_context::executor_type> strand_;
};


int main() {


	return 0;
}