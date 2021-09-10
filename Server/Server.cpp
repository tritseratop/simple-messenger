#include "Server.h"
#include <iostream>
#include <thread>
//#include <utility>

void ServerClientHandler(SOCKET client) {
}

void Server::AddClient(Socket client) {
    client_it[client.GetSocketHandle()] = client_sockets.insert(client_sockets.end(), client);
}

Result Server::StartListen(Endpoint endpoint) {
	return main_socket.Listen(endpoint);
}

Result Server::StartListen(std::string ip, int port) {
	return main_socket.Listen(ip, port);
}

void Server::HandleClients() {
    fd_set master;
    FD_ZERO(&master);
    FD_SET(main_socket.GetSocketHandle(), &master);

    while (true) {
        fd_set copy = master;
        int socket_count = select(0, &copy, nullptr, nullptr, nullptr);
        client_count = socket_count - 1;
        for (int i = 0; i < socket_count; ++i) {
            Socket sock(copy.fd_array[i]);
            if (sock.GetSocketHandle() == main_socket.GetSocketHandle()) {
                Socket client;
                Result res = main_socket.Accept(client);
                if (res == Result::Error) {
                    int error = WSAGetLastError();
                    break;
                }
                client_it[client.GetSocketHandle()] = client_sockets.insert(client_sockets.end(), client);

                FD_SET(client.GetSocketHandle(), &master);

                std::string welcomeMsg = "Welcome to the Awesome Chat Server!\r";

                client.Send(welcomeMsg);
                //send(client.GetSocketHandle(), welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
            }
            else {
                std::string message;
                if (sock.Recv(message) != Result::Success) {
                    FD_CLR(sock.GetSocketHandle(), &master);
                    DeleteSocket(sock);
                }
                else {
                    SendToAll(message, sock);
                }
            }
        }
    }
}

Result Server::SendToAll(std::string msg, Socket from) {
    for (auto& s : client_sockets) {
        if (from.GetSocketHandle() != s.GetSocketHandle()) {
            std::string msg_to_send = "Client #" + std::to_string(from.GetSocketHandle()) + " " + msg;
            if (s.Send(msg_to_send) != Result::Success) {
                DeleteSocket(s);
            }
        }
    }
    return Result::Success;
}

void Server::DeleteSocket(Socket& s) {
    int handle = s.GetSocketHandle();
    s.Close();
    client_sockets.erase(client_it.at(handle)); //в каком порядке?
    client_it.erase(handle);
    
}