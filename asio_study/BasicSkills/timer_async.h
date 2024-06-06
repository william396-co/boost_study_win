#pragma once
#include <iostream>

#include <boost/asio.hpp>

namespace timer2 {
	inline void print(boost::system::error_code const&) {
		std::cout << "Hello Boost(with async wait 5 seconds)\n";
	}

	inline void timer_async() {
		std::cout << __FUNCTION__ << "\n";
		boost::asio::io_context io;
		boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

		t.async_wait(&print);
		io.run();//like thread.join()
	}
}