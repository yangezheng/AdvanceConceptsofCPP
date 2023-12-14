#include "server.h"

namespace net{

Server::Server(uint16_t port) : socket_() {
    // Initialize the server socket and start listening on the specified port
    socket_.listen(port);
}

Connection Server::accept() const {
    // Accept a new connection and return a Connection object
    FileDescriptor client_fd = socket_.accept().fd_;
    
    // Return a Connection object taking ownership of the file descriptor
    return Connection(FileDescriptor(client_fd));
}



}