#include "client.h"

namespace net
{
    
void Client::connect(uint16_t port) {
    // Connect to the specified port on localhost
    socket_.connect(port);
}

void Client::connect(std::string destination, uint16_t port) {
    // Connect to the specified destination address and port
    socket_.connect(destination, port);
}
    
} // namespace net
