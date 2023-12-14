#include "client.h"

namespace net
{

// Define the connect functions outside the class
Connection Client::connect(uint16_t port) {
    // Connect to the specified port on localhost
    // FileDescriptor socket_fd = socket_.connect(port).fd_;

    // Return a Connection object representing the connection
    return Connection(socket_.connect(port).fd_);
}

Connection Client::connect(std::string destination, uint16_t port) {
    // Connect to the specified destination address and port and get the file descriptor
    // FileDescriptor socket_fd = socket_.connect(destination, port).fd_;

    // Return a Connection object taking ownership of the file descriptor
    return Connection(socket_.connect(destination, port).fd_);
}


} // namespace net
