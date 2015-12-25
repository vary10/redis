#include <iostream>
#include "socket.h"

Socket::Socket(int ini_sd) {
    sd = ini_sd;
}

Socket::~Socket() {
    close(sd);
}

int Socket::get_sd() {
    return sd;
}

void Socket::send_data(char* input, size_t len) {
    size_t size = 0;
    while ((size += write(sd, input, len - size)) < len) {}
}

size_t Socket::get_data(char* out, size_t size) {
    return read(sd, out, size);
}
