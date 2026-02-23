#include "states.h"
#include "myNetUtil.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(54321);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cerr << "Bind failed." << endl;
        return 1;
    }

    listen(serverSocket, 5); 
    cout << "Server listening on port 54321....." << endl;

  while (true) { 
        sockaddr_in clientAddress;
        socklen_t clientLen = sizeof(clientAddress);
        
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
        cout << "Connected to a client!" << endl;

       string abbrev;
       if (receiveFrame(clientSocket, abbrev)) {
        string response = getStateName(abbrev);

        if (response.empty()) {
            response = "Error!!! Invalid state abbreviation. Please enter a valid state abbreviation";
        }

        sendFrame(clientSocket, response);
        cout << "processed: " << abbrev << " : " << response << endl;
       }

        close(clientSocket);
        cout << "Connection closed. Waiting for next client.....";
    }
    close(serverSocket);
    return 0;
}