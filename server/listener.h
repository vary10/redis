#pragma once

#include <iostream>
#include <memory>
#include "socket.h"

class Listener {
private:
    int ld;
    int port_id;
    int max_clients;
public:
    Listener(int port, unsigned int  clients);
    ~Listener();
    std::unique_ptr<Socket> accept_client();
};

