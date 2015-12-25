#include "Server.h"

Server::Server(int port, unsigned int clients) : my_listener(port, clients) {}

void Server::my_serve() {

    std::cout << "start" << std::endl;
    std::unique_ptr<Socket> client = my_listener.accept_client();
    std::cout << "got client" << std::endl;

    SocketReader from_client(client->get_sd());
    SocketWriter to_client(client->get_sd());
    RedisValue err = RedisError("I'm not a real redis");
    while (true) {
        RedisValue val;
        ReadRedisValue(&from_client, &val);
        std::cout << "read " << std::endl;
        WriteRedisValue(&to_client, err);
        // WriteRedisValue(&to_client, val);
        to_client.flush();
        std::cout << "wrote " << std::endl;
        from_client.read_more();
    }
}

Server::~Server() {
    my_listener.~Listener();
}
