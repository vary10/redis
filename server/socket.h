#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string>
#include <stdexcept>
#include <unistd.h>


class Socket {
private:
    int sd;
public:
    struct sockaddr_in options;
    Socket(int sd);
    ~Socket();
    int get_sd();
    void send_data(char* input, size_t len);
    size_t get_data(char* out, size_t size);
};
