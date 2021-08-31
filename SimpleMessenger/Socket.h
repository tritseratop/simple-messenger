#pragma once
#include "Result.h"
#include "Endpoint.h"

class Socket {
public:
	Socket(SOCKET handle = INVALID_SOCKET);
	Result Close();
	Result Create();
	Result Bind(Endpoint endpoint);
	Result Listen(Endpoint endpoint, int backlog = 5);
	Result Accept(Socket& outSocket);
	Result Connect(Endpoint endpoint);
	Result Send(const void* data, int numberOfBytes, int& bytesSent);
	Result Recv(void* data, int numberOfBytes, int& bytesSent);
	SOCKET GetSocketHandle();
private:
	SOCKET handle = INVALID_SOCKET;
};