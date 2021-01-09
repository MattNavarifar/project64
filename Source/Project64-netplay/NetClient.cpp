#include "NetClient.h"

#include "trace.h"

#include <vector>
#include <asio.hpp>

NNetClient::NNetClient()
{
	auto ClientEventLoop = []() {
		asio::io_context io_context;
		asio::ip::tcp::resolver resolver{ io_context };
		asio::ip::tcp::resolver::results_type results =
			resolver.resolve(
				asio::ip::tcp::v4(), 
				"localhost", 
				"daytime"
			);
		asio::ip::tcp::socket socket(io_context);
		asio::connect(socket, results);

		for (;;)
		{
			std::vector<char> buf(128);
			asio::error_code ec;
			size_t len = socket.read_some(
				asio::buffer(buf), 
				ec
			);
			if (ec == asio::error::eof)
			{
				break; //connection has been closed.
			}
			else if (ec)
			{
				//todo: handle this.
			}
		}
	};

	m_ClientThread = std::move(std::thread(ClientEventLoop));

	
}

NNetClient::~NNetClient()
{
}
