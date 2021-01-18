#include "NetServer.h"
//#include <Project64-core/N64System/Mips/MemoryVirtualMem.h>
#include <ctime>
#include <iostream>
#include <thread>
#include <mutex>


std::mutex mtx;
uint32_t INITIAL_CAPACITY = 1000;
uint8_t LOAD_FACTOR = 2;

std::string MakeDaytimeString()
{
	std::time_t now = time(0);
	return ctime(&now);
}

class ByteBuffer {
public:
	ByteBuffer() :
		Capacity(INITIAL_CAPACITY),
		m_Size(0),
		Buffer(new byte[INITIAL_CAPACITY])
	{
	}
	~ByteBuffer() {
		delete Buffer;
	}
	void AddToBuffer(const byte* const& data, const uint16_t& size)
	{
		const uint32_t remainingSize = Capacity - m_Size;
		if (remainingSize <= 0 || remainingSize < size)
		{
			IncreaseCapacity(size * LOAD_FACTOR);
		}
		memcpy(Last(), data, size);
		m_Size += size;
	}

	inline byte* Data()
	{
		return Buffer;
	}

	inline uint32_t Size()
	{
		return m_Size;
	}

private:
	ByteBuffer(const ByteBuffer& other);
	ByteBuffer& operator=(const ByteBuffer& other);

	void IncreaseCapacity(uint32_t amount)
	{
		const byte* tmp = Buffer;
		const uint32_t NewCapacity = Capacity + amount;
		Buffer = new byte[NewCapacity];
		memcpy(Buffer, tmp, NewCapacity);
		Capacity = NewCapacity;
	}

	byte* const Last()
	{
		return Buffer + m_Size;
	}

private:
	uint32_t Capacity;
	uint32_t m_Size;
	byte* Buffer;

};

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

	void Send(ByteBuffer* buf)
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
			asio::buffer(buf->Data(), buf->Size()),
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
	tcp_server(asio::io_context& io_context, ByteBuffer*& buffer) :
		m_Acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 7133)),
		m_PendingBuffer(buffer)
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
				if (m_PendingBuffer->Size() > 0)
				{
					std::make_shared<tcp_connection>(socket)->Send(m_PendingBuffer);
				}
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
	ByteBuffer* m_PendingBuffer;
};

NNetServer::NNetServer(void)
{
	PendingBuffer = new ByteBuffer();
	auto NetworkingEventLoop = [=]() {
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

NNetServer::~NNetServer()
{
	delete PendingBuffer;
}

void NNetServer::SendData(const byte* const& data, const uint16_t&& size)
{
	PendingBuffer->AddToBuffer(data, size);
}
