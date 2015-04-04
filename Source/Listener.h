#pragma once

#include "Socket.h"

namespace rokunet {

class Listener : public Socket {
public:
    Listener();

    /**
     *  Blocks until a socket connection can be accepted.
     *  @return Whether the connection was succesful or not.
     */
    bool accept(Socket* socket) noexcept;
    bool listen(unsigned short port) noexcept;
};

} // namespace rokunet

