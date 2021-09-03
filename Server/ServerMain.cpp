#include <WinSock2.h>
#include <iostream>
#include <future>
#include <sstream>
#include "Server.h"

int main(int argc, char* argv[]) {
    Server server;
    bool res = server.Initialize();
    if (server.Create() == Result::Success) {
        std::cout << "Server socket successfully created!" << std::endl;
        if (server.StartListen() == Result::Success) {
            std::cout << "Server socket successfully started listening on " << PORT << std::endl;
            server.HandleClients();
        }
        else {
            std::cerr << "Failed to listen on port 4790" << std::endl;
        }
        server.Close();
    }
    server.Shutdown();
    system("pause");
    return 0;
}