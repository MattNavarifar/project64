#pragma once

#include <thread>

class NNetClient {
public:
	NNetClient();
	~NNetClient();
	void Start(uint8_t* ram);
private:
	std::thread m_ClientThread;
};