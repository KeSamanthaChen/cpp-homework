#include "filedescriptor.h"
#include <unistd.h>

namespace net {
    FileDescriptor::FileDescriptor() : fd_(-1) {}

    FileDescriptor::FileDescriptor(int fd) : fd_(fd) {}

    FileDescriptor::~FileDescriptor() {
        if (fd_.has_value() && fd_.value()!=-1) {
            close(fd_.value());
        }
    }

    FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept : fd_(other.fd_) {
        other.fd_ = -1;
    }

    FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) noexcept {
        if (&other == this) return *this;
        fd_ = other.fd_;
        other.fd_ = -1;
        return *this;
    }

    int FileDescriptor::unwrap() const {
        if (fd_.has_value()) {
            return fd_.value();
        } else {
            return -1;
        }
    }

}
