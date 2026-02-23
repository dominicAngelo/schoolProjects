#ifndef NET_UTIL_H
#define NET_UTIL_H
#include <string>
#include <sys/socket.h>
#include <stdexcept>

// keeping functions inline so they dont have to be defined in both server and client

inline bool receiveAll(int sock, void* buff, int length) { // functions derived from online code examples and the textbook
    int remaining = length; 
    char* data = (char*)buff;
    while (remaining > 0) {
        int received = recv(sock, data, remaining, 0);
        if (received <= 0) {
            return false;
        }
        data += received;
        remaining -= received;
    }
    return true;
}

inline bool sendAll(int sock, const void* buff, int length) {
    char* data = (char*)buff;
    while (length > 0) {
        int sent = send(sock, data, length, 0);
        if (sent <= 0) return false;
        data += sent;
        length -= sent;
    }
    return true;
}

inline bool sendFrame(int socket, const string& message) {
    uint16_t length = htons((uint16_t)message.size());

    if(!sendAll(socket, &length, sizeof(length))) {
        return false;
    }
    if (message.size() > 0 && !sendAll(socket, message.data(), message.size())) {
        return false;
    }
    return true;
}

inline bool receiveFrame(int sock, string &out) {
    uint16_t totalLength;
    if (!receiveAll(sock, &totalLength, sizeof(totalLength))) {
        return false;
    }
    uint16_t length = ntohs(totalLength);
    out.resize(length);
    if (length > 0 && !receiveAll(sock, &out[0], length)) {
        return false;
    }
    return true;
}

#endif