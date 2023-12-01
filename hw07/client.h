#pragma once

#include "connection.h"
#include "socket.h"

namespace net {

/**
 * TCP socket client. Can connect to a server given a destination IP address and a port.
 *
 * TODO:
 * - A client must be default constructible
 * - A client must have a function `connect` which connects on a given port on the localhost  
 * - A client must have a function `connect` which connects to a given destination address and port
 */
class Client {
};

} // namespace net
