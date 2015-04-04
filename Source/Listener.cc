#include "Listener.h"

namespace rokunet {

Listener::Listener() {
    setTimeout(0);
}

bool Listener::accept(Socket* socket) {
    socket->handle = ::accept(handle,
                              static_cast<sockaddr*>(nullptr),
                              static_cast<socklen_t*>(nullptr));
    return true;
}

bool Listener::listen(unsigned short port) {
    auto address = createAddress("0.0.0.0", port);
    if (bind(handle,
             reinterpret_cast<sockaddr*>(&address),
             sizeof(address)) == 0) {
        if (::listen(handle, 0) == 0) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

} // namespace rokunet

