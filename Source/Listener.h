#pragma once

#include "Socket.h"

namespace rokunet {

class Listener : public Socket {
public:
    Listener();

    bool accept(Socket* socket) noexcept;
    bool listen(unsigned short port) noexcept;
};

} // namespace rokunet

