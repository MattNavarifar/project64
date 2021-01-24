/****************************************************************************
*                                                                           *
* Project64-Netplay					                                        *
* http://www.pj64-emu.com/                                                  *
* Copyright (C) 2021 Project64. All rights reserved.                        *
*                                                                           *
* License:                                                                  *
* GNU/GPLv2 http://www.gnu.org/licenses/gpl-2.0.html                        *
*																			*
* @author - Matt Navarifar													*
*                                                                           *
****************************************************************************/
#pragma once
#include <string>
#include <asio.hpp>
#include <vector>
#include <utility>

typedef uint8_t byte;
typedef int address;

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

	inline uint32_t GetSizeBytes() const
	{
		return sizeof(m_Start) + sizeof(m_Size) + m_Size;
	}

private:
	const uint32_t m_Start;
	const byte* const m_Ram;
	const uint16_t m_Size;
};

namespace std {
	class thread;
}


class DataBuffer;

class NNetServer {
public:
	NNetServer(void);
	~NNetServer();
	void SendMemoryRange(const MemRange & memRange);

private:
	std::thread m_ServerThread;
	DataBuffer* PendingBuffer;
	//std::vector<MemRange> m_SendMemRanges;


};

