#include "Socket.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

namespace rokunet {

Socket::Socket(const Domain domain, const Type type, const Protocol protocol) {
    handle = socket(domainToInt(domain), typeToInt(type),
                    protocolToInt(protocol));
    setTimeout(5);
}

Socket::~Socket() {
    disconnect();
}

void Socket::send(const std::string& data) noexcept {
    ::send(handle, data.data(), data.size(), 0);
}

std::string Socket::receive(const unsigned int size) noexcept {
    char buffer[size];
    auto received = recv(handle, buffer, size, 0);
    return std::string(buffer, received);
}

bool Socket::connect(const std::string& host,
                     const unsigned short port) noexcept {
    auto address = createAddress(host, port);
    return ::connect(handle,
                     reinterpret_cast<sockaddr*>(&address),
                     sizeof(address)) == 0;
}

bool Socket::disconnect() noexcept {
    return ::close(handle) == 0;
}

sockaddr_in Socket::createAddress(const std::string& host,
                                  const unsigned short port) const noexcept {
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));

    inet_pton(AF_INET, host.data(), &(address.sin_addr));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    return address;
}

void Socket::setTimeout(const long seconds, const long microseconds) noexcept {
    struct timeval tv;
    tv.tv_sec = seconds;
    tv.tv_usec = microseconds;
    setsockopt(
        handle, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)
    );
}

void Socket::setTimeout(const long seconds) noexcept {
    setTimeout(seconds, 0);
}

std::string Socket::getRemoteAddress() const noexcept {
    sockaddr_in address;

    socklen_t size = sizeof(address);
    getpeername(handle, reinterpret_cast<sockaddr*>(&address), &size);

    char buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, buffer, sizeof(buffer));

    return std::string(buffer);
}

unsigned short Socket::getLocalPort() const noexcept {
    sockaddr_in address;
    socklen_t size = sizeof(address);

    getsockname(handle, reinterpret_cast<sockaddr*>(&address), &size);

    return ntohs(address.sin_port);
}

int Socket::domainToInt(const Domain domain) {
    int cDomain;
    switch (domain) {
    case Domain::Local:
        cDomain = AF_LOCAL;
        break;
    case Domain::Inet:
        cDomain = AF_INET;
        break;
    case Domain::Inet6:
        cDomain = AF_INET6;
        break;
    case Domain::IPX:
        cDomain = AF_IPX;
        break;
    case Domain::Netlink:
        cDomain = AF_NETLINK;
        break;
    case Domain::X25:
        cDomain = AF_X25;
        break;
    case Domain::AX25:
        cDomain = AF_AX25;
        break;
    case Domain::ATMPVC:
        cDomain = AF_ATMPVC;
        break;
    case Domain::AppleTalk:
        cDomain = AF_APPLETALK;
        break;
    case Domain::Packet:
        cDomain = AF_PACKET;
        break;
    case Domain::ALG:
        cDomain = AF_ALG;
        break;
    }
    return cDomain;
}

int Socket::typeToInt(const Type type) {
    int cType;
    switch (type) {
    case Type::Stream:
        cType = SOCK_STREAM;
        break;
    case Type::Dgram:
        cType = SOCK_DGRAM;
        break;
    case Type::SeqPacket:
        cType = SOCK_SEQPACKET;
        break;
    case Type::Raw:
        cType = SOCK_RAW;
        break;
    case Type::RDM:
        cType = SOCK_RDM;
        break;
    }
    return cType;
}

int Socket::protocolToInt(const Protocol protocol) {
    int cProtocol;
    switch (protocol) {
    case Protocol::IP:
        cProtocol = IPPROTO_IP;
        break;
    case Protocol::ICMP:
        cProtocol = IPPROTO_ICMP;
        break;
    case Protocol::TCP:
        cProtocol = IPPROTO_TCP;
        break;
    case Protocol::EGP:
        cProtocol = IPPROTO_EGP;
        break;
    case Protocol::PUP:
        cProtocol = IPPROTO_PUP;
        break;
    case Protocol::UDP:
        cProtocol = IPPROTO_UDP;
        break;
    case Protocol::IDP:
        cProtocol = IPPROTO_IDP;
        break;
    case Protocol::TP:
        cProtocol = IPPROTO_TP;
        break;
    case Protocol::Raw:
        cProtocol = IPPROTO_RAW;
        break;
    case Protocol::Max:
        cProtocol = IPPROTO_MAX;
        break;
    }
    return cProtocol;
}

} // namespace rokunet

