#include <WinSock2.h>
#include <iostream>
#include <future>
#include <sstream>
#include "SimpleMessenger\IncludeMe.h"
#pragma warning(disable: 4996)

const int MAX_CLIENT_COUNT = 5;
SOCKET Connections[MAX_CLIENT_COUNT];
int count = 0;

void ClientHandler(int index) {
    int msg_size = 0;
    while (true) {
        int res = recv(Connections[index], (char*)&msg_size, sizeof(msg_size), NULL);
        if (res == SOCKET_ERROR) {
            closesocket(Connections[index]);
            break;
        }
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connections[index], msg, msg_size, NULL);
        for (int i = 0; i < count; ++i) {
            if (i == index) {
                continue;
            }
            else {
                send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
                send(Connections[i], msg, msg_size, NULL);
            }
        }
        delete[] msg;
    }
}



int main(int argc, char* argv[]) {
    WSAData wsaData;                                                        // Загрузка необходимой версии библиотеки
    WORD DLLVersion = MAKEWORD(2, 1);                                       // Запрашиваемая версия библиотеки WinSock
    if (WSAStartup(DLLVersion, &wsaData) != 0) {                            // Загружает библиотеку
        std::cout << "DLL loading failed" << std::endl;
        exit(1);
    }

    Socket listening;
     if (listening.Create() == Result::Success) {

        SOCKADDR_IN addr;                                                       // Хранит информацию об адресе сокета
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP
        addr.sin_port = htons(4790);                    // Port
        addr.sin_family = AF_INET;

        if (listening.Listen(Endpoint((sockaddr*)&addr)) == Result::Success) {
            std::cout << "Socket successfully listening on 4790" << std::endl;

            fd_set master;
            FD_ZERO(&master);
            FD_SET(listening.GetSocketHandle(), &master);
            while (true) {
                fd_set copy = master;
                int socket_count = select(0, &copy, nullptr, nullptr, nullptr);
                for (int i = 0; i < socket_count; ++i) {
                    SOCKET sock = copy.fd_array[i];
                    
                    if (sock == listening.GetSocketHandle()) {
                        SOCKET client = accept(listening.GetSocketHandle(), nullptr, nullptr);
                        FD_SET(client, &master);

                        std::string welcomeMsg = "Welcome to the Awesome Chat Server!\r\n";
                        send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
                    }
                    else {
                        char buf[4096];
                        ZeroMemory(buf, 4096);
                        int bytesIn = recv(sock, buf, 4096, 0);
                        if (bytesIn == SOCKET_ERROR) {
                            closesocket(sock);
                        }
                        else {
                            for (int i = 0; i < master.fd_count; i++)
                            {
                                SOCKET outSock = master.fd_array[i];
                                if (outSock != listening.GetSocketHandle() && outSock != sock)
                                {
                                    std::ostringstream ss;
                                    ss << "SOCKET #" << sock << ": " << buf << "\r\n";
                                    std::string strOut = ss.str();

                                    int res = send(outSock, strOut.c_str(), strOut.size() + 1, 0);
                                    if (res == SOCKET_ERROR) {
                                        closesocket(sock);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            std::cerr << "Failed to list on port 4790" << std::endl;
        }
        listening.Close();
    }
    else {
        std::cerr << "Socket failed to create" << std::endl;
    }

    system("pause");
    return 0;
}