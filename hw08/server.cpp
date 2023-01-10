#include "server.h"

namespace net {
    Server::Server() {}

    Server::Server(uint16_t port) : sock_{Socket{}} {
        sock_.listen(port);
    }

    Connection Server::accept() const {
        return sock_.accept();
    }
}