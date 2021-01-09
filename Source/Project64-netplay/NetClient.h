#pragma once

#include <thread>

class NNetClient {
public:
	NNetClient();
	~NNetClient();
private:
	std::thread m_ClientThread;
};