#pragma once

#include <stdint.h>
#include "connection.h"
#include "socket.h"

namespace net {

/**
 * TCP socket server. Listens for your request to deliver you juicy data!
 */
class Server {
    public:
        Server();

        Server(uint16_t port);

        Connection accept() const;

    private:
        Socket sock_;
};

} // namespace net
