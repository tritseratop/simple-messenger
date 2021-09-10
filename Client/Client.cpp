#include "Client.h"
#include <thread>
#include <future>

void Client::Receiving() {
	std::string message;
	while (true) {
		main_socket.Recv(message);
		std::cout << message;
	}
}

Result Client::Connect(Endpoint endpoint) {
	return main_socket.Connect(endpoint);
}

Result Client::Connect(std::string ip, int port) {
	return main_socket.Connect(Endpoint(ip.c_str(), port));
}

// ÆÈÅÑÒÜ
Result Client::StartChating() {
	std::string rec_msg;
	std::string send_msg;
	std::future<void> f = std::async(
		std::launch::async,
		[this](std::string& message) {
			while (true) {
				if (main_socket.Recv(message) == Result::Error) {
					int error = WSAGetLastError();
					return;
				}
				std::cout << message << std::endl;
			}
		}, 
		std::ref(rec_msg)
	);
	while (true) {
		std::getline(std::cin, send_msg);
		if (main_socket.Send(send_msg) == Result::Error) {
			return Result::Error;
		}
	}
}