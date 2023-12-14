#pragma once

#include <optional>

namespace net {

/// Wrapper for a Linux style file descriptor. It represents a potentially empty file descriptor.
/// The wrapper represents a unique ownership model, i.e. the file descriptor will get invalided
/// with the end of the lifetime of the object.
///
/// The following code should help you understand the ownership model:
/// ```cpp
/// auto fdraw {get_socket_fd_from_somewhere()};
/// {
///     FileDescriptor fd{fdraw};
///
///     // use fd, do whatever with it
/// }
/// // fd should not be usable anymore
/// ```
class FileDescriptor {
public:
    /// Default constructor for empty file descriptor
    FileDescriptor();

    /// Construct from a integer file descriptor returned from the C API, take ownership of the
    /// descriptor
    explicit FileDescriptor(int fd);

    /// Close the file descriptor (if present and valid)
    /// Check out: close(3)
    ~FileDescriptor();

    // TODO: Implement both copy and move constructors and assignment operators for the ownership model
    //       described in the class description.

    // Copy constructor
    FileDescriptor(const FileDescriptor& other) = delete;

    //Copy assignment operator
    FileDescriptor& operator=(const FileDescriptor& other) = delete;
    
    // Move constructor
    FileDescriptor(FileDescriptor&& other) noexcept;

    //Move assignment operator
    FileDescriptor& operator=(FileDescriptor&& other) noexcept;


    /// Return the underlying file descriptor, if not present return -1 (this is quite standard for
    /// linux systems)
    int unwrap() const;


    // Additional Methods
    
    // Check if the file descriptor is valid
    bool valid() const;

    // Close the file descriptor
    void close();

private:
    std::optional<int> fd_ {};
};
} // namespace net
