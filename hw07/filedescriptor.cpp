#include "filedescriptor.h"
#include <cstdio>
#include <unistd.h>

// TODO Implement the methods of the FileDescriptor class as given in the filedescriptor.h header.


namespace net{

FileDescriptor::FileDescriptor() : fd_(std::nullopt) {}

FileDescriptor::FileDescriptor(int fd) : fd_(fd) {}

FileDescriptor::~FileDescriptor(){
    close();
}

FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept : fd_(std::exchange(other.fd_, std::nullopt)) {}

FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) noexcept {
    if (this != &other) {
        close();
        fd_ = std::exchange(other.fd_, std::nullopt);
    }
    return *this;
}


bool FileDescriptor::valid() const {
    return fd_.has_value();
}

int FileDescriptor::unwrap() const {
    return valid() ? fd_.value() : -1;
}

void FileDescriptor::close() {
    if (valid()) {
        ::close(fd_.value());
        fd_ = std::nullopt;
    }
}

}