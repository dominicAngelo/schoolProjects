#include <iostream>
#include <string>
#include "myNetUtil.h"
#include "states.h"

using namespace std;

int main() {
    
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error!! WSAStartup failed!!" << endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress = {0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(54321);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "Bind failed!! Error, " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, SOMAXCONN);
    cout << "Server listening on port 54321....." << endl;

    while (true) {
        sockaddr_in clientAddress;
        int clientLen = sizeof(clientAddress);
        
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientLen);
        if (clientSocket == INVALID_SOCKET) { 
            cerr << "Accept failed!! Error: " << WSAGetLastError() << endl;
            continue;
        } else {
            cout << "Connected to a client!" << endl;
        }

        cout << "Connected to a client!" << endl;

        string abbrev, response;

        if (receiveFrame(clientSocket, abbrev)) {
            response = getStateName(abbrev);

            if (response.empty()) {
                response = "Error!!! Invalid state abbreviation!! Please try again with something valid";
            }

            sendFrame(clientSocket, response);
            cout << "Processed: " << abbrev << " : " << response << endl;
        }

        closesocket(clientSocket);
        cout << "Connection closed, ready for next client" << endl;
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}