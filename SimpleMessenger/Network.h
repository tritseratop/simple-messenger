#pragma once

#include "Socket.h"
#include <iostream>
#include <string>

class Network {
public:
	Network() {}
	bool Initialize();
	void Shutdown();
	Result Create();
	Result Close();
	Socket GetMainSocket();

protected:
	Socket main_socket;
};