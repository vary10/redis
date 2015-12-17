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

void Socket::send_data(std::string&& input) {
    size_t size = 0;
    while ((size += write(sd, input.c_str(), input.size() - size)) < input.size()) {}
}

std::string* Socket::get_data(size_t size) {
    std::string* out = new std::string();
    out->resize(size);
    ssize_t total_read, temp_read;
    temp_read = read(sd, const_cast<char*>(out->c_str()), size);
    total_read = temp_read;
    while (total_read < size && temp_read) {
        temp_read = read(sd, const_cast<char*>(out->c_str()), size - total_read);
        total_read += temp_read;
    }
    std::cout << "Total size of input: " << size << std::endl;
    out->resize(size);
    return out;
}
