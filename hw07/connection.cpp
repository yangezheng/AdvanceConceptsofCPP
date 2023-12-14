#include "connection.h"
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <ostream>

namespace net{

[[nodiscard]] ssize_t send(int fd, std::span<const char> data) {
    ssize_t sent = ::send(fd, data.data(), data.size(), 0);
    if (sent == -1) {
        perror("Send error");
    }
    return sent;
}

[[nodiscard]] ssize_t receive(int fd, std::span<char> buf) {
    ssize_t received = ::recv(fd, buf.data(), buf.size(), 0);
    if (received == -1) {
        perror("Receive error");
    }
    return received;
}

Connection::Connection(FileDescriptor&& fd): fd_(std::move(fd)){}


void Connection::send(std::string_view data) const {
    ssize_t sent = ::send(fd_.unwrap(), data.data(), data.size(), 0);
    if (sent == -1) {
        perror("Send error");
    }
}

void Connection::send(std::istream& data) const {
    const size_t buffer_size = 128;
    char buffer[buffer_size];

    while (data.good()) {
        data.read(buffer, buffer_size);
        ssize_t sent = ::send(fd_.unwrap(), buffer, data.gcount(), 0);
        if (sent == -1) {
            perror("Send error");
            break;
        }
    }
}

ssize_t Connection::receive(std::ostream& stream) const {
    const size_t buffer_size = 128;
    char buffer[buffer_size];

    ssize_t received = ::recv(fd_.unwrap(), buffer, buffer_size, 0);

    if (received > 0) {
        stream.write(buffer, received);
    } else if (received == 0) {
        // Connection closed by the other end
        return 0;
    } else {
        perror("Receive error");
    }

    return received;
}

ssize_t Connection::receive_all(std::ostream& stream) const {
    const size_t buffer_size = 128;
    char buffer[buffer_size];

    ssize_t total_received = 0;

    while (true) {
        ssize_t received = ::recv(fd_.unwrap(), buffer, buffer_size, 0);

        if (received > 0) {
            stream.write(buffer, received);
            total_received += received;
        } else if (received == 0) {
            // Connection closed by the other end
            break;
        } else {
            perror("Receive error");
        }
    }

    return total_received;
}


int Connection::fd() const {
    return fd_.unwrap();
}

}