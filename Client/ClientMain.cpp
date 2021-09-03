#include <WinSock2.h>
#include <iostream>
#include <string>
#include "SimpleMessenger\IncludeMe.h"
#include "Client.h"

int main(int argc, char* argv[]) {
    Client client;
    bool res = client.Initialize();
    if (client.Create() == Result::Success) {
        std::cout << "Client socket successfully created!" << std::endl;
        if (client.Connect() == Result::Success) {
            std::cout << "Client connected!" << std::endl;
            client.StartChating();
        }
        else {
            std::cerr << "Failed to connect to the server!" << std::endl;
        }
        client.Close();
    }
    client.Shutdown();

    system("pause");
    return 0;
}