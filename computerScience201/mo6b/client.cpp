#include <iostream>
#include <string>
#include "myNetUtil.h"

using namespace std;

int main(int argc, char* argv[]) {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    string serverIP = "127.0.0.1";
    int port = 54321;

    if (argc >= 2) {
        serverIP = argv[1];
    } else if (argc >= 3) {
        port = stoi(argv[2]);
    }

    cout << "State Lookup Client IP: " << serverIP << " on port " << port << endl;

    string input, response;

    while (true) {
        cout << "Enter state abbreviation or q to quit: ";

        if (!getline(cin, input) || input.empty()) { 
            break;
        }

        for (char &c : input) {
            c = toupper(c);
        }

        if (input == "Q" || input == "EXIT") {
             break;
        }

        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (sock == INVALID_SOCKET) {
            cerr << "Socket creation failed." << endl;
            continue;
        }

        sockaddr_in address = {0};
        address.sin_family = AF_INET;
        address.sin_port = htons((u_short)port);
        
        inet_pton(AF_INET, serverIP.c_str(), &address.sin_addr);

        if (connect(sock, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
            cerr << "Connection failed!! Error: " << WSAGetLastError() << endl;
            closesocket(sock);
            continue;
        }

        if (sendFrame(sock, input)) {
            if(receiveFrame(sock, response)) {
                cout << "Result: " << response << "\n" << endl;
            }
        }

        closesocket(sock);
    }

    WSACleanup();
    return 0;
}