#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

namespace rokunet {

class Socket {
public:
    Socket();
    ~Socket();

    void send(const std::string& data) noexcept;
    std::string receive(unsigned int size) noexcept;
    bool connect(const std::string& host, unsigned short port) noexcept;
    bool disconnect() noexcept;
    void setTimeout(long seconds, long microseconds) noexcept;
    void setTimeout(long seconds) noexcept;
    std::string getRemoteAddress() const noexcept;
    unsigned short getLocalPort() const noexcept;
    
    int handle;

protected:
    sockaddr_in
    createAddress(const std::string& host, unsigned short port) const noexcept;
};

} // namespace rokunet

