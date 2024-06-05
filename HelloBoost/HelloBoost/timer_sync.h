#pragma once

#include <iostream>
#include <boost/asio.hpp>

namespace timer1 {
	inline void timer_sync() {

		std::cout << __FUNCTION__ << "\n";
		boost::asio::io_context io;
		boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

		t.wait();

		std::cout << "Hello Boost(with sync wait 5 seconds)\n";
	}
}