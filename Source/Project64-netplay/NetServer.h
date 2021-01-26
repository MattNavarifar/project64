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

namespace std {
	class thread;
}

struct MemRange;
class DataBuffer;

class NNetServer {
public:
	NNetServer(void);
	~NNetServer();
	void Start();
	void SendMemoryRange(const MemRange & memRange, uint8_t* ram);

private:
	std::thread m_ServerThread;
	DataBuffer* PendingBuffer;
	//std::vector<MemRange> m_SendMemRanges;


};

