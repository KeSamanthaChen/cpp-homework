#include "client.h"

namespace net {
    Client::Client() {}

    Connection Client::connect(std::string destination, uint16_t port) {
        return sock_.connect(destination, port);
    }

    Connection Client::connect(uint16_t port) {
        return sock_.connect(port);
    }
}