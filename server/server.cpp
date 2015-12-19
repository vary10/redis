#include "Server.h"


Server::Server(int port, unsigned int clients) : my_listener(port, clients) {}

void Server::my_serve() {
    RedisValue val;
    std::deque<std::unique_ptr<Socket>> clients_deque;
    clients_deque.push_back(my_listener.accept_client());

    SocketReader from_client(clients_deque[0]->get_sd());
    SocketWriter to_client(clients_deque[0]->get_sd());

    while (true) {
        ReadRedisValue(&from_client, &val);
        WriteRedisValue(&to_client, val);
        to_client.flush();
    }
}