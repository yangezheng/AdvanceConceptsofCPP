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


// Implementation of the move constructor
FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept {
    // Move the file descriptor and invalidate the other FileDescriptor
    fd_ = std::move(other.fd_);
    other.fd_.reset();
}

// Implementation of the move assignment operator
FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) noexcept {
    if (this != &other) {
        // Release the current file descriptor
        fd_.reset();
        // Move the file descriptor and invalidate the other FileDescriptor
        fd_ = std::move(other.fd_);
        other.fd_.reset();
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