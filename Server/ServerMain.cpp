#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <list>
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
    WSAData wsaData;                                                        // �������� ����������� ������ ����������
    WORD DLLVersion = MAKEWORD(2, 1);                                       // ������������� ������ ���������� WinSock
    if (WSAStartup(DLLVersion, &wsaData) != 0) {                            // ��������� ����������
        std::cout << "DLL loading failed" << std::endl;
        exit(1);
    }

    Socket server;
     if (server.Create() == Result::Success) {
        SOCKADDR_IN addr;                                                       // ������ ���������� �� ������ ������
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP
        addr.sin_port = htons(4790);                    // Port
        addr.sin_family = AF_INET;
        if (server.Listen(Endpoint((sockaddr*)&addr)) == Result::Success) {
            std::cout << "Socket successfully listening on 4790" << std::endl;
            std::list<std::thread> client_threads;
            for (int i = 0; i < 5; ++i) {   // ������� � ���������
                Socket client;
                if (server.Accept(client) == Result::Success) {
                    Connections[i] = client.GetSocketHandle();
                    ++count;
                    std::cout << "Client connected!" << std::endl;

                    int msg_size = 0;

                    //int res = recv(Connections[i], (char*)&msg_size, sizeof(msg_size), NULL);
                    //char* name = new char[msg_size + 1];
                    //name[msg_size] = '\0';
                    //recv(Connections[i], name, msg_size, NULL);
                    //std::string hello_msg(name);
                    //hello_msg += " joined to chat";
                    //msg_size = hello_msg.size();
                    //int bytesReceived = 0;
                    //Result res = client.Send(&hello_msg, msg_size, bytesReceived);

                    char msg[256] = "Hello. It's my first network program!\n";
                    int bytesReceived = 0;
                    Result res = client.Send(&msg, sizeof(msg), bytesReceived);
                    client_threads.push_back(std::thread(ClientHandler, i));
                    client_threads.back().detach();
                    //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)i, NULL, NULL);
                }
                else {
                    std::cerr << "Failed to accept new connection" << std::endl;
                }
            }
        }
        else {
            std::cerr << "Failed to list on port 4790" << std::endl;
        }
        server.Close();
    }
    else {
        std::cerr << "Socket failed to create" << std::endl;
    }

    /*
    SOCKET client;
    for (int i = 0; i < MAX_CLIENT_COUNT; ++i) {
        client = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);     // ������������� ���������� � ��������
        if (client == 0) {
            std::cout << "Connection failed" << std::endl;
            exit(1);
        }
        else {
            Connections[i] = client;
            ++count;
            std::cout << "Client connected!" << std::endl;
            char msg[256] = "Hello. It's my first network program!\n";
            send(client, msg, sizeof(msg), NULL);                    // �������� ��������� �������

            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)i, NULL, NULL);
        }
    }
    */
    system("pause");
    return 0;
}