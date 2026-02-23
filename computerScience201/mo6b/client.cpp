#include <iostream>
#include "myNetUtil.h"
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int DEFAULT_PORT = 54321;

int main(int argc, char* argv[]) {

    string response;

    string serverIP = "127.0.0.1";
    int port = 54321;

    if (argc >= 2) serverIP = argv[1]; // if specific IP nad port values are given assign them here
    if (argc >= 3) port = stoi(argv[2]); 

    cout << "State Lookup Client IP: " << serverIP << " on port " << port << endl;

    string input;

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

        int socket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in address = {0};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);

        inet_pton(AF_INET, serverIP.c_str(), &address.sin_addr);

        if (connect(socket, (sockaddr*)&address, sizeof(address)) < 0) {
            perror("Connect failed");
            close(socket);
            continue;
        }

        if (sendFrame(socket, input)) {
            if(receiveFrame(socket, response)) {
                cout << "Result: " << response << "\n" << endl;
            }
        }

        close(socket);
    }

    return 0;
}