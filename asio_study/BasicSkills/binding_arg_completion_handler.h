#pragma once

#include <iostream>
#include <functional>
#include <boost/asio.hpp>

namespace timer3 {

	inline void print(boost::system::error_code const&, boost::asio::steady_timer* t, int* count) {

		if (*count < 5) {
			std::cout << *count << "\n";
			++(*count);
			t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
			t->async_wait(std::bind(print, boost::asio::placeholders::error, t, count));
		}
	}

	inline void binding_arg_completion_handler() {

		boost::asio::io_context io;
		int count = 0;
		boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
		t.async_wait(std::bind(print, boost::asio::placeholders::error, &t, &count));
		io.run();

		std::cout << "Final count is " << count << "\n";
	}

}