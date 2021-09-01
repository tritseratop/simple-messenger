#include "Server.h"
#include <iostream>
#include <thread>

void ServerClientHandler(SOCKET client) {
}

bool Server::Initialize() {
	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (result != 0) {
		std::cerr << "Failed to start up the winsock API." << std::endl;
		return false;
	}
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		std::cerr << "Could not find a usable version of the winsock API dll." << std::endl;
		return false;
	}
	std::cout << "Winsock API successfully initialized!" << std::endl;
	return true;
}
void Server::Shutdown() {
	WSACleanup();
}
void Server::AddClient(Socket client) {
	clients.push_back(client);
}

Result Server::StartListen(Endpoint endpoint) {
	return server.Listen(endpoint);
}

void Server::HandleClients() {
	for (int i = 0; i < 5; ++i) {   // вынести в константу
		Socket client;
		if (server.Accept(client) == Result::Success) {
			clients[i] = client.GetSocketHandle();
			++count;
			std::cout << "Client connected!" << std::endl;

			char msg[256] = "Hello. It's my first network program!\n";
			int bytesReceived = 0;
			Result res = client.Send(&msg, sizeof(msg), bytesReceived);

			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ServerClientHandler, (LPVOID)i, NULL, NULL);
		}
		else {
			std::cerr << "Failed to accept new connection" << std::endl;
		}
	}
}