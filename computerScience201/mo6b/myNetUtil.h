#ifndef NET_UTIL_H
#define NET_UTIL_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>

#pragma comment(lib, "ws2_32.lib")

// we dont want a separate .cpp file so keeping functions inline

inline void closeSocket(SOCKET sock) {
    closesocket(sock);
}

inline bool receiveAll(SOCKET sock, void* buff, int length) {
    int remaining = length;
    char* data = (char*)buff;
    
    while (remaining > 0) {
        int received = recv(sock, data, remaining, 0);
        if (received <= 0) return false;
        data += received;
        remaining -= received;
    }
    return true;
}

inline bool sendAll(SOCKET sock, const void* buff, int length) {
    const char* data = (const char*)buff;

    while (length > 0) {
        int sent = send(sock, data, length, 0);

        if (sent <= 0) {
            return false;
        }

        data += sent;
        length -= sent;
    }
    return true;
}

inline bool sendFrame(SOCKET sock, const std::string& message) {
    uint16_t length = htons((uint16_t)message.size());

    if (!sendAll(sock, &length, sizeof(length))){
        return false;
    }
    if (message.size() > 0 && !sendAll(sock, message.data(), (int)message.size())) {
        return false;
    } else {
        return true; 
    }
}

inline bool receiveFrame(SOCKET sock, std::string &out) {
    uint16_t totalLength;

    if (!receiveAll(sock, &totalLength, sizeof(totalLength))) { 
        return false;
    }

    uint16_t length = ntohs(totalLength);
    out.resize(length);

    if (length > 0 && !receiveAll(sock, &out[0], length)) {
        return false;
    } else {
        return true;
    }
}
#endif