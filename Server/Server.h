#pragma once
#include "SimpleMessenger\IncludeMe.h"
#include <iostream>
#include <list>
#include <thread>

class Server {
public:
	Server() : clients(5) {
		if (server.Create() != Result::Success) {
			std::cerr << "Socket failed to create" << std::endl;
		}
	}
	Result StartListen(Endpoint endpoint);
	bool Initialize();
	void Shutdown();
	void HandleClients();
	//friend void ClientHandler(Server& server);
	void AddClient(Socket client);
private:
	std::vector<Socket> clients;
	Socket server;
	size_t count = 0;
	std::list<std::thread> cliend_handler_threads;
};