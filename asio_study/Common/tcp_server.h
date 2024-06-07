#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "utils.h"

using boost::asio::ip::tcp;

class tcp_connection : public std::enable_shared_from_this< tcp_connection> {
public:
	using pointer = std::shared_ptr<tcp_connection>;
	static pointer create(boost::asio::io_context& io) {
		return pointer(new tcp_connection{ io });
	}
	tcp::socket& socket() {
		return socket_;
	}
	void start() {
		message_ = utils::make_daytime_string();
		boost::asio::async_write(socket_,
			boost::asio::buffer(message_),
			boost::bind(&tcp_connection::handle_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}
private:
	explicit tcp_connection(boost::asio::io_context& io) :
		socket_{ io } {}

	void handle_write(boost::system::error_code const& error, size_t len) {
		std::cout << __FUNCTION__ << "( " << error << "," << len << ")\n";
	}
private:
	tcp::socket socket_;
	std::string message_{};
};

class tcp_server {
public:
	tcp_server(boost::asio::io_context& io, int16_t port)
		:io_context_{ io },
		acceptor_{ io,tcp::endpoint(tcp::v4(),port) }
	{
		start_accept();
	}

private:
	void start_accept() {
		tcp_connection::pointer new_connection
			= tcp_connection::create(io_context_);
		acceptor_.async_accept(new_connection->socket(),
			boost::bind(&tcp_server::handle_accept, this, new_connection,
				boost::asio::placeholders::error));
	}
	void handle_accept(tcp_connection::pointer new_connection, boost::system::error_code const& error) {
		if (!error) {
			new_connection->start();
		}
		start_accept();
	}

private:
	boost::asio::io_context& io_context_;
	tcp::acceptor acceptor_;
};