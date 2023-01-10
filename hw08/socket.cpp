#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <iostream>
#include <cstring> //memeset
#include <netdb.h> //addrinfo
#include "socket.h"
#include "filedescriptor.h"

namespace net {
    bool is_listening(int fd) {
        // return true;
        int val;
        socklen_t len = sizeof(val);
        if (getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &val, &len) == -1) {
            printf("fd %d is not a socket\n", fd);
            return false;
        }
        else if (val) {
            printf("fd %d is a listening socket\n", fd);
            return true;
        }
        else {
            printf("fd %d is a non-listening socket\n", fd);
            return false;
        }
    }

    Socket::Socket() {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0); // hope this will work at once
        if (sockfd != -1) {
            fd_ = std::move(FileDescriptor{sockfd}); // move assignment
        }
    }
    
    void Socket::listen(uint16_t port) const {
        int yes = 1;
        int sockfd = fd_.unwrap();
        // std::cout << "listen sockfd: " << sockfd << std::endl;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            std::cout << "setsockopt failed" << std::endl;
            throw std::runtime_error{"setsockopt failed"};
        }

        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        if (bind(sockfd, (struct sockaddr*)&address, sizeof(address)) == -1) {
            std::cout << "bind failed" << std::endl;
            throw std::runtime_error{"bind failed"};
        }

        if (::listen(sockfd, 10) == -1) { // global version
            std::cout << "listen failed" << std::endl;
            throw std::runtime_error{"listen failed"};
        }
    }

    Connection Socket::accept() const {
        int sockfd = fd_.unwrap();

        if (!is_listening(sockfd)) {
            throw std::runtime_error{"socket is not listening"};
        }

        struct sockaddr_storage accept_addr;
        socklen_t accept_addr_size = sizeof(accept_addr);
        int new_sockfd = ::accept(sockfd, (struct sockaddr*)&accept_addr, &accept_addr_size);
        Connection cn{std::move(FileDescriptor{new_sockfd})};
        return cn; // for the server, the new_sockfd is for per connection(client)
    }

    Connection Socket::connect(std::string destination, uint16_t port) {
        struct addrinfo hints;
        struct addrinfo *res;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        const char * hostname = destination.c_str();
        const char * port_char = std::to_string(port).c_str();
        getaddrinfo(hostname, port_char, &hints, &res);

        int sockfd = fd_.unwrap();
        int client_fd = ::connect(sockfd, res->ai_addr, res->ai_addrlen); // maybe need the loop
        freeaddrinfo(res);
        // closed problem here
        Connection cn{std::move(FileDescriptor{sockfd})}; // maybe should be sockfd
        fd_ = std::move(FileDescriptor{});
        return cn; // for the client, the client_fd is in no use I guess
    }

    Connection Socket::connect(uint16_t port) {
        return connect("localhost", port);
    }

    int Socket::fd() const {
        return fd_.unwrap();
    }
}