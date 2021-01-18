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

typedef uint8_t byte;

namespace std {
	class thread;
}


class ByteBuffer;

class NNetServer {
public:
	NNetServer(void);
	~NNetServer();
	void SendData(const byte*const & data, const uint16_t&& size);

private:
	std::thread m_ServerThread;
	ByteBuffer* PendingBuffer;


};

