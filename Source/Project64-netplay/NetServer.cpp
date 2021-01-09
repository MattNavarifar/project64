#include "NetServer.h"
#include <ctime>
#include <iostream>
#include <thread>

std::string MakeDaytimeString()
{
	std::time_t now = time(0);
	return ctime(&now);
}

class tcp_connection : public std::enable_shared_from_this<tcp_connection>{
public:
	typedef std::shared_ptr<tcp_connection> pointer;

	tcp_connection(asio::ip::tcp::socket& socket)
		: m_Socket(std::move(socket))
	{
	}

	asio::ip::tcp::socket& socket()
	{
		return m_Socket;
	}

	void start()
	{
		auto self = shared_from_this();
		auto writeHandler = [this, self](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				HandleWrite(length);
			}
		};
		m_Message = MakeDaytimeString();
		asio::async_write(
			m_Socket,
			asio::buffer(m_Message),
			writeHandler
		);
	}

private:

	void HandleWrite(std::size_t length)
	{
		if (length <= 0)
		{
			std::cerr << "blah" << std::endl;
		}
	}

	asio::ip::tcp::socket m_Socket;
	std::string m_Message;
};

class tcp_server {
public:
	tcp_server(asio::io_context& io_context) :
		m_Acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13))
	{
		StartAccept();
	}
private:
	/*
	* Creates a socket and initiates an asynchronous accept operation to wait for a new connection.
	*/
	void StartAccept()
	{

		auto acceptHandler = [this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::make_shared<tcp_connection>(socket)->start();
				HandleAccept();
			}
		};
		
		m_Acceptor.async_accept(acceptHandler);
	}

	void HandleAccept()
	{
		StartAccept();
	}
	
private:
	asio::io_context m_Io_Context;
	asio::ip::tcp::acceptor m_Acceptor;
};

NNetServer::NNetServer(void)
{
	auto NetworkingEventLoop = []() {
		try {
			asio::io_context io_context;
			tcp_server server(io_context);
			io_context.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	};

	m_ServerThread = std::move(std::thread(NetworkingEventLoop));
}

NNetServer::~NNetServer()
{

}
