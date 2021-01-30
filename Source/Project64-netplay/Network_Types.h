#pragma once

#include <stdint.h>
#include <string>
#include <mutex>

typedef uint8_t byte;

/*enum NetSerializableType
{
	Type_MemRange = 0
};

//template<NetSerializableType type>
class NetSerializable
{
public:
	virtual ~NetSerializable() = 0;
	virtual void Serialize(byte* const buffer) const = 0;
	//virtual uint32_t GetSizeBytes() const = 0;
};*/

struct MemRange
{
	MemRange() :
		m_Start(0),
		m_Size(0)
	{

	}

	MemRange operator=(const MemRange& other)
	{
		return MemRange(other.m_Start, other.m_Size);
	}

	MemRange(const uint32_t start, const uint16_t size) :
		m_Start(start),
		m_Size(size)
	{
	}
	~MemRange()
	{
	}

	inline bool IsValid()
	{
		return m_Size; // part of our contract states we won't consider a valid MemRange with a size of 0. This is optimial for quickly terminating linear scans of MemRanges in a Vector like we do in tcpConnection.
	}

	virtual void Serialize(byte* const buffer) const
	{
		byte* begin = buffer;
		memcpy(begin, &m_Start, sizeof(m_Start));
		begin += sizeof(m_Start);
		memcpy(begin, &m_Size, sizeof(m_Size));
	}

	static MemRange Deserialize(byte* const buffer)
	{
		byte* begin = buffer;
		uint32_t start;
		memcpy(&start, begin, sizeof(uint32_t));
		begin += sizeof(uint32_t);
		uint16_t size;
		memcpy(&size, begin, sizeof(uint16_t));
		return MemRange(start, size);
	}

	uint32_t GetSizeBytes() const
	{
		return sizeof(m_Start) + sizeof(m_Size);
	}

	inline uint16_t Size() const
	{
		return m_Size;
	}

	inline uint32_t GetStart() const
	{
		return m_Start;
	}

private:
	const uint32_t m_Start;
	const uint16_t m_Size;
};

constexpr uint32_t INITIAL_CAPACITY = 1000;
constexpr uint8_t LOAD_FACTOR = 2;

class DataBuffer {
public:
	DataBuffer() :
		Capacity(INITIAL_CAPACITY),
		m_Size(0),
		Buffer(new byte[INITIAL_CAPACITY]),
		m_Mutex()
	{
	}
	~DataBuffer() {
		delete Buffer;
	}
	void AddToBuffer(const MemRange& netSerial, byte* ram)
	{
		m_Mutex.lock();
		const uint32_t memRangeSizeBytes = netSerial.GetSizeBytes();
		const uint32_t remainingSize = Capacity - m_Size;
		if (remainingSize <= 0 || remainingSize < memRangeSizeBytes)
		{
			IncreaseCapacity(netSerial.GetSizeBytes() * LOAD_FACTOR);
		}
		byte* begin = Last();
		netSerial.Serialize(begin);
		begin += memRangeSizeBytes;
		memcpy(begin, ram + netSerial.GetStart(), netSerial.Size());
		m_Size += memRangeSizeBytes + netSerial.Size();
		m_Mutex.unlock();
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
		m_Mutex.lock();
		if (!IsEmpty())
		{
			memset(Data(), 0, m_Size);
			m_Size = 0;
		}
		m_Mutex.unlock();
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
	std::mutex m_Mutex; //lock to prevent the server thread from clearing out the buffer while the mem_watcher thread adds to it.

};
