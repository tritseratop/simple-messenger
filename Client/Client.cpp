#include <WinSock2.h>
#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

SOCKET Connection;

void ClientHandler() {
    int msg_size = 0;
    while (true) {
        recv(Connection, (char*)&msg_size, sizeof(int), NULL);
        char* msg = new char[msg_size + 1];
        msg[msg_size] = '\0';
        recv(Connection, msg, msg_size, NULL);
        std::cout << msg << std::endl;
        delete[] msg;
    }
}

int main(int argc, char* argv[]) {
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "DLL loading failed" << std::endl;
        exit(1);
    }
    SOCKADDR_IN addr;

    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(4790);
    addr.sin_family = AF_INET;

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {  // Соединение с сервером
        std::cout << "Error: failed connect to server! \n";
        exit(1);
    }

    std::cout << "Client connected! \n";
    char msg[256];
    recv(Connection, msg, sizeof(msg), NULL);                       // Прием сообщения
    std::cout << msg;

    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

    std::string message;
    while (true) {
        std::getline(std::cin, message);
        int message_size = message.size();
        send(Connection, (char*)&message_size, sizeof(int), NULL);
        send(Connection, message.c_str(), message_size, NULL);
        Sleep(10); // ???
    }

    system("pause");
    return 0;
}