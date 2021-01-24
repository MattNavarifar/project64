#pragma once

#include <thread>

class NNetClient {
public:
	NNetClient(uint8_t* ram);
	~NNetClient();
private:
	std::thread m_ClientThread;
	uint8_t* m_Memory;
};