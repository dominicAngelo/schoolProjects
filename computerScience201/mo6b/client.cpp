#include "states.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <cstdint>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int DEFAULT_PORT = 54321;

bool sendAll(int sock, const void* buf, int len) {
    const char* ptr = (const char*)buf;
    while (len > 0) {
        int sent = send(sock, ptr, len, 0);
        if (sent <= 0) {
            return false;
        }
        ptr += sent;
        len -= static_cast<int>(sent);
    }
    return true;
}

bool receiveAll(int sock, void* buf, int len) {
    char* ptr = (char*)buf;
    while (len > 0) {
        int got = recv(sock, ptr, len, 0);
        if (got <= 0) {
            return false;
        }
        ptr += got;
        len -= got;
    }
    return true;
}

bool sendFrame(int sock, string s) {
    uint16_t len = htons((uint16_t)s.size());
    if (!sendAll(sock, &len, 2)) {
        return false;
    }
    if (s.size() > 0 && !sendAll(sock, s.data(), s.size())) {
        return false;
    }
    return true;
}

bool receiveFrame(int sock, string &out) {
    uint16_t netLen;
    if (!receiveAll(sock, &netLen, 2)) {
        return false;
    }
    int len = ntohs(netLen);
    out.resize(len);
    if (len > 0 && !receiveAll(sock, &out[0], len)) {
        return false;
    }
    return true;
}

void trim(string& s) {
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    s.erase(s.find_last_not_of(" \t\n\r") + 1);
}

int main(int argc, char* argv[]) {
    string serverIP = (argc >= 2) ? argv[1] : "127.0.0.1";
    int port = (argc >= 3) ? atoi(argv[2]) : DEFAULT_PORT;

    cout << "State Lookup Client(" << serverIP << ":" << port << ")" << endl;
    string input;
    while (cout << "Enter state or q to quit: " && getline(cin, input)) {
        trim(input);
        if (input == "q" || input == "exit" || input.empty()) {
            break;
        }

        for (char &c : input) {
            c = toupper(c);
        }

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, serverIP.c_str(), &addr.sin_addr);

        if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("Connect failed");
            close(sock);
            continue;
        }

        if (sendFrame(sock, input)) {
            string response;
            if (receiveFrame(sock, response)) {
                cout << "Result: " << response << "\n" << endl;
            }
        }

        close(sock);
    }

    return 0;
}