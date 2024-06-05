#pragma once

#include <functional>
#include <iostream>
#include <boost/asio.hpp>

namespace timer4 {

	class printer {
	public:
		explicit printer(boost::asio::io_context& io):
			timer_{ io,boost::asio::chrono::seconds{1} },
			count_{} {
			timer_.async_wait(std::bind(&printer::print, this));
		}
		~printer() {
			std::cout << "Final count is " << count_ << "\n";
		}
		void print() {
			if (count_ < 5) {
				std::cout << count_ << "\n";
				++count_;
				timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds{ 1 });
				timer_.async_wait(std::bind(&printer::print, this));
			}
		}
	private:
		boost::asio::steady_timer timer_;
		int count_;
	};

	void memfn_completion_handler() {
		boost::asio::io_context io;
		printer p(io);
		io.run();
	}

}