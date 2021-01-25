#include "NetClient.h"

#include "trace.h"
#include "Network_Types.h"

#include <vector>
#include <asio.hpp>

NNetClient::NNetClient()
{
	
}

NNetClient::~NNetClient()
{
}

void NNetClient::Start(uint8_t* ram)
{
	auto ClientEventLoop = [=]() {
		asio::io_context io_context;
		asio::ip::tcp::resolver resolver{ io_context };
		asio::ip::tcp::resolver::results_type results =
			resolver.resolve(
				asio::ip::tcp::v4(),
				"localhost",
				"7133"
			);
		asio::ip::tcp::socket socket(io_context);
		asio::connect(socket, results);

		for (;;)
		{
			std::vector<byte> buf(128);
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
			else
			{
				MemRange memRange = MemRange::Deserialize(buf.data(), ram);
				if (memRange.Size() > 0)
				{
					byte* data = buf.data() + sizeof(uint32_t) + sizeof(uint16_t);
					memcpy(ram + memRange.GetStart(), data, memRange.Size());
				}
			}
		}
	};

	m_ClientThread = std::move(std::thread(ClientEventLoop));
}
