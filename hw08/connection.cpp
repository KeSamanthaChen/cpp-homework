#include <sys/socket.h>

#include "connection.h"

namespace net {
    ssize_t send(int fd, std::span<const char> data) {
        return ::send(fd, data.data(), data.size(), 0);
    }

    ssize_t receive(int fd, std::span<char> buf) {
        return recv(fd, buf.data(), buf.size(), 0);
    }

    // it is not a move constructor...
    Connection::Connection(FileDescriptor&& fd) noexcept : fd_(std::move(fd)) {}

    // send string_view data
    void Connection::send(std::string_view data) const {
        net::send(fd(), std::span<const char>(data.data(), data.size()));
    }

    void Connection::send(std::istream& data) const {
        std::istreambuf_iterator<char> eos;
        std::string s(std::istreambuf_iterator<char>(data), eos);
        net::send(fd(), std::span<const char>(s.data(), s.size()));
    }

    ssize_t Connection::receive(std::ostream& stream) const {
        char buf[128];
        ssize_t bytes =  net::receive(fd(), std::span<char>(buf, 128));
        stream.write(buf, bytes);
        return bytes;
    }

    // problem now...
    ssize_t Connection::receive_all(std::ostream& stream) const {
        ssize_t total{0};
        ssize_t bytes{0};
        char buf[128];
        std::string receive_string;
        while((bytes = net::receive(fd(), std::span<char>(buf, 128))) > 0) {
            total += bytes;
            receive_string.append(buf, bytes);
            // stream.write(buf, bytes);
        }
        stream.write(receive_string.c_str(), receive_string.size());
        return total;
    }

    int Connection::fd() const {
        return fd_.unwrap();
    }
}
