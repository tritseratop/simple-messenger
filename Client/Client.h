#pragma once
#include "SimpleMessenger\IncludeMe.h"
#include <iostream>
#include <list>
#include <queue>
#include <string>

class Client : public Network {
public:
	Client() {}
	Result Connect(Endpoint endpoint);
	Result Connect(std::string ip = IP, int port = PORT);
	Result StartChating();
	void Receiving();
private:
	std::list<Socket> client_sockets;
	std::queue<Socket> waiting_clients; // TODO очередь ожидания
	size_t client_count = 0;
};
