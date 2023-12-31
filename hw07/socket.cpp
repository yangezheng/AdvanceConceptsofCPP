#include "socket.h"

#include <iostream>
#include <stdexcept>
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <optional>
#include <netinet/tcp.h>



namespace net{

bool is_listening(int fd) {
    int optval;
    socklen_t optlen = sizeof(optval);

    // Use getsockopt to retrieve the SO_ACCEPTCONN option
    if (getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &optval, &optlen) == -1) {
        // Error handling: throw an exception or return false
        throw std::runtime_error("getsockopt failed");
    }

    // Check the value of the option to determine if it's a listening socket
    return optval != 0;
}

Socket::Socket() : fd_() {
    // Initialize the socket with the appropriate type and protocol
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Socket creation error");
        throw std::runtime_error("Failed to create socket");
    }

    fd_ = FileDescriptor(socket_fd);
    
    const int enable = 1;
    if (setsockopt(fd_.unwrap(), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        throw std::runtime_error("error setting SO_REUSEADDR for socket");
    }

    int flag = 1;
    if (setsockopt(fd_.unwrap(), IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)) < 0) {
        std::cerr << "Could not set TCP_NODELAY for socket\n";
    }

}


// Socket::Socket()
// : fd_(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) {
//     if (fd_.unwrap() < 0) {
//         throw std::runtime_error("error creating socket");
//     }

//     const int enable = 1;
//     if (setsockopt(fd_.unwrap(), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
//         throw std::runtime_error("error setting SO_REUSEADDR for socket");
//     }

//     int flag = 1;
//     if (setsockopt(fd_.unwrap(), IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int)) < 0) {
//         std::cerr << "Could not set TCP_NODELAY for socket\n";
//     }
// }


void Socket::listen(uint16_t port) const {
    if (!fd_.valid()) {
        throw std::runtime_error("Socket is not valid");
    }

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    // Bind the socket to the specified address and port
    if (::bind(fd_.unwrap(), reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1) {
        perror("Bind error");
        throw std::runtime_error("Failed to bind socket");
    }

    // Start listening for incoming connections
    if (::listen(fd_.unwrap(), SOMAXCONN) == -1) {
        perror("Listen error");
        throw std::runtime_error("Failed to listen on socket");
    }
}

Connection Socket::accept() const {
    if (!fd_.valid()) {
        throw std::runtime_error("Socket is not valid");
    }

    if (!is_listening(fd_.unwrap())) {
        throw std::runtime_error("Socket is not listening");
    }

    // Accept a new connection and return a Connection object
    int client_fd = ::accept(fd_.unwrap(), nullptr, nullptr);
    if (client_fd == -1) {
        perror("Accept error");
        throw std::runtime_error("Failed to accept connection");
    }

    return Connection(FileDescriptor(client_fd));
}


// connect

Connection Socket::connect(std::string destination, uint16_t port) {
    if (!fd_.valid()) {
        throw std::runtime_error("Socket is not valid");
    }

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, destination.c_str(), &addr.sin_addr) <= 0) {
        struct hostent* host_entry = gethostbyname(destination.c_str());
        if (host_entry == nullptr) {
            throw std::runtime_error("Failed to resolve host");
        }
        std::memcpy(&addr.sin_addr, host_entry->h_addr, host_entry->h_length);
    }

    // Connect to the specified address and port
    if (::connect(fd_.unwrap(), reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1) {
        perror("Connect error");
        throw std::runtime_error("Failed to connect to server");
    }


    return Connection(std::move(fd_));
    // return Connection(FileDescriptor(fd_.unwrap()));
}


// Connection Socket::connect(std::string destination, uint16_t port) {
//     struct sockaddr_in server { }; server.sin_family = AF_INET; server.sin_port = htons(port);

//     // parse or resolve ip addr
//     if (uint32_t addr = inet_addr(destination.c_str()); addr != INADDR_NONE) {
//         std::memcpy(&server.sin_addr, &addr, sizeof(addr));
//     } else {
//         // TODO: use getaddrinfo and support IPv6 :)
//         hostent* host_info = gethostbyname(destination.c_str());
//         if (host_info == nullptr) {
//             throw std::runtime_error("destination host could not be resolved");
//         }
//         std::memcpy(&server.sin_addr, host_info->h_addr, static_cast<size_t>(host_info->h_length));
//     }

//     if (::connect(fd_.unwrap(), reinterpret_cast<sockaddr*>(&server), sizeof(server)) < 0) {
//         throw std::runtime_error("failed to connect to server (is it running?)");
//     }

//     // transfer the fd into the connection
//     return Connection { std::move(fd_) };
//     }


Connection Socket::connect(uint16_t port) {
    return connect("localhost", port);
}

int Socket::fd() const {
    return fd_.unwrap();
}

}