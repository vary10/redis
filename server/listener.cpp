#include "listener.h"


Listener::Listener(int port, unsigned int clients) {
    port_id = port;
    max_clients = clients;
    ld = socket(PF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(ld, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_id);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(ld, (struct sockaddr*) &addr, sizeof(addr));

    listen(ld, max_clients);
}

Listener::~Listener() {
    close(ld);
}

std::unique_ptr<Socket> Listener::accept_client() {
    struct sockaddr_in addr;
    socklen_t length = sizeof(addr);

    std::unique_ptr<Socket> new_client(new Socket(accept(ld, (struct sockaddr *) &addr, &length)));
    new_client->options = addr;
    return new_client;
}
