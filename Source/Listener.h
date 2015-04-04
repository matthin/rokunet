#pragma once

#include "Socket.h"

namespace rokunet {

class Listener : public Socket {
public:
    Listener();

    bool accept(Socket* socket);
    bool listen(unsigned short port);
};

} // namespace rokunet

