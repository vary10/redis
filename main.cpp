#include <iostream>

#include "logic/redis.h"
#include "server/server.h"

int main() {
    Server srv;
    srv.my_serve();
    return 0;
}
