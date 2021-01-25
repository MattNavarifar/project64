#pragma once

#include <stdint.h>
#include <string>

typedef uint8_t byte;


struct MemRange
{
	MemRange() :
		m_Start(0),
		m_Size(0),
		m_Ram(nullptr)
	{

	}

	MemRange operator=(const MemRange& other)
	{
		return MemRange(other.m_Start, other.m_Size, other.m_Ram);
	}

	MemRange(const uint32_t start, const uint16_t size, const uint8_t* const ram) :
		m_Start(start),
		m_Size(size),
		m_Ram(ram)
	{
	}
	~MemRange()
	{
	}

	inline bool IsValid()
	{
		return m_Size; // part of our contract states we won't consider a valid MemRange with a size of 0. This is optimial for quickly terminating linear scans of MemRanges in a Vector like we do in tcpConnection.
	}

	void Serialize(byte* const buffer) const
	{
		byte* begin = buffer;
		memcpy(begin, &m_Start, sizeof(m_Start));
		begin += sizeof(m_Start);
		memcpy(begin, &m_Size, sizeof(m_Size));
		begin += sizeof(m_Size);
		memcpy(begin, m_Ram + m_Start, m_Size);
	}

	static MemRange Deserialize(byte* const buffer, const byte* const ram)
	{
		byte* begin = buffer;
		const uint32_t start = (uint32_t)(*begin);
		begin += sizeof(uint32_t);
		const uint16_t size = (uint16_t)(*begin);
		begin += sizeof(uint16_t);
		return MemRange(start, size, ram);
	}

	inline uint32_t GetSizeBytes() const
	{
		return sizeof(m_Start) + sizeof(m_Size) + m_Size;
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
	const byte* const m_Ram;
	const uint16_t m_Size;
};

constexpr uint32_t INITIAL_CAPACITY = 1000;
constexpr uint8_t LOAD_FACTOR = 2;

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
