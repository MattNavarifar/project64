#include "NetServer.h"
//#include <Project64-core/N64System/Mips/MemoryVirtualMem.h>
#include <ctime>
#include <iostream>
#include <thread>
#include <mutex>


std::mutex mtx;
uint32_t INITIAL_CAPACITY = 1000;
uint8_t LOAD_FACTOR = 2;


class DataBuffer {
public:
	DataBuffer() :
		Capacity(INITIAL_CAPACITY),
		m_Size(0),
		Buffer(new byte[INITIAL_CAPACITY])
	{
	}
	~DataBuffer() {
		delete Buffer;
	}
	void AddToBuffer(const MemRange& memRange)
	{
		const uint32_t memRangeSizeBytes = memRange.GetSizeBytes();
		const uint32_t remainingSize = Capacity - m_Size;
		if (remainingSize <= 0 || remainingSize < memRangeSizeBytes)
		{
			IncreaseCapacity(memRange.GetSizeBytes() * LOAD_FACTOR);
		}
		memRange.Serialize(Last());
		m_Size += memRangeSizeBytes;
	}

	inline byte* Data() const
	{
		return Buffer;
	}

	inline uint32_t Size() const
	{
		return m_Size;
	}

	void Clear()
	{
		if (!IsEmpty())
		{
			memset(Data(), 0, m_Size);
			m_Size = 0;
		}
	}

	inline bool IsEmpty() { return m_Size == 0; }

private:
	DataBuffer(const DataBuffer& other);
	DataBuffer& operator=(const DataBuffer& other);

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
		auto writeHandler = [this, self](std::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				OnWriteComplete();
			}
		};

		asio::async_write(
			m_Socket,
			asio::buffer(m_SendBuffer->Data(), m_SendBuffer->Size()),
			writeHandler
		);
		
	}

private:

	void OnWriteComplete()
	{
		//todo: handle write success here.
		m_SendBuffer->Clear();
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

NNetServer::~NNetServer()
{
	delete PendingBuffer;
}

void NNetServer::SendMemoryRange(const MemRange& memRange)
{
	//m_SendMemRanges.emplace_back(memRange);
	PendingBuffer->AddToBuffer(memRange);
}

