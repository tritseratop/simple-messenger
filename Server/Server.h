#pragma once
#include "SimpleMessenger\IncludeMe.h"
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <string>

class Server : public Network {
public:
	Server() {}
	Result StartListen(Endpoint endpoint);
	Result StartListen(std::string ip = IP, int port = PORT);
	void HandleClients();
	//friend void ClientHandler(Server& server);
	void AddClient(Socket client);
private:
	std::list<Socket> client_sockets;
	std::map<int, std::list<Socket>::iterator> client_it;
	std::queue<Socket> waiting_clients;
	//Socket server_socket;
	size_t client_count = 0;
	Result SendToAll(std::string msg, Socket from);
	void DeleteSocket(Socket& s);
	//fd_set master;
};