#include "NetServer.h"
//#include <Project64-core/N64System/Mips/MemoryVirtualMem.h>
#include <ctime>
#include <iostream>
#include <thread>

#include "Network_Types.h"

class tcp_connection : public std::enable_shared_from_this<tcp_connection>{
public:
	typedef std::shared_ptr<tcp_connection> pointer;

	tcp_connection(asio::ip::tcp::socket& socket, DataBuffer* & sendBuffer)
		: m_Socket(std::move(socket)),
		m_SendBuffer(sendBuffer)
	{
	}

	asio::ip::tcp::socket& socket()
	{
		return m_Socket;
	}

	void SendBuffer()
	{
		auto self = shared_from_this();
		auto writeHandler = [this, self](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				OnWriteComplete(length);
			}
		};

		asio::async_write(
			m_Socket,
			asio::buffer(m_SendBuffer->Data(), m_SendBuffer->Size()),
			writeHandler
		);
		
	}

private:

	void OnWriteComplete(std::size_t length)
	{
		//todo: handle write success here.
		if (length > 0)
		{
			m_SendBuffer->Clear();
		}
		SendBuffer();
	}

	asio::ip::tcp::socket m_Socket;
	std::string m_Message;
	DataBuffer* m_SendBuffer;
};

class tcp_server {
public:
	tcp_server(asio::io_context& io_context, DataBuffer* & sendBuffer) :
		m_Acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 7133)),
		m_SendBuffer(sendBuffer)
	{
		StartAccept();
	}
private:

	void AcceptConnection()
	{

	}
	/*
	* Creates a socket and initiates an asynchronous accept operation to wait for a new connection.
	*/
	void StartAccept()
	{

		auto acceptHandler = [this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::make_shared<tcp_connection>(socket, m_SendBuffer)->SendBuffer();
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
	DataBuffer* m_SendBuffer;
};

NNetServer::NNetServer(void)
{
	PendingBuffer = new DataBuffer();
	
}

NNetServer::~NNetServer()
{
	delete PendingBuffer;
}

void NNetServer::Start()
{
	auto NetworkingEventLoop = [&]() {
		try {
			asio::io_context io_context;
			tcp_server server(io_context, PendingBuffer);
			io_context.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	};

	m_ServerThread = std::move(std::thread(NetworkingEventLoop));
}

void NNetServer::SendMemoryRange(const MemRange& memRange, uint8_t* ram)
{
	PendingBuffer->AddToBuffer(memRange, ram);
}

