#include "server.h"

namespace net{

Server::Server(uint16_t port) : socket_() {
    // Initialize the server socket and start listening on the specified port
    socket_.listen(port);
}

Connection Server::accept() const {
    // Accept a new connection and return a Connection object
    return socket_.accept();
}

}