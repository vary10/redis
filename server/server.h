#pragma once

#include <deque>
#include "Listener.h"
#include "../logic/redis.h"


class Server {
protected:
    Listener my_listener;
public:
    explicit Server(int port = 6379, unsigned int clients = 10);
    virtual void my_serve() = 0;
};
