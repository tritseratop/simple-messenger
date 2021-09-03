#include "Network.h"

bool Network::Initialize() {
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

void Network::Shutdown() {
	WSACleanup();
}

Result Network::Create() {
	return main_socket.Create();
}

Result Network::Close() {
	return main_socket.Close();
}

Socket Network::GetMainSocket() {
	return main_socket;
}