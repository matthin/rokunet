#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

namespace rokunet {

class Socket {
public:
    enum Domain {
        Local,
        Inet,
        Inet6,
        IPX,
        Netlink,
        X25,
        AX25,
        ATMPVC,
        AppleTalk,
        Packet,
        ALG
    };

    enum Type {
        Stream,
        Dgram,
        SeqPacket,
        Raw,
        RDM
    };

    enum struct Protocol {
        IP,
        ICMP,
        TCP,
        EGP,
        PUP,
        UDP,
        IDP,
        TP,
        Raw,
        Max
    };

    Socket(
        Domain domain = Inet,
        Type type = Stream,
        Protocol protocol = Protocol::IP
    );
    ~Socket();

    /**
     * Sends message to the connected server.
     */
    void send(const std::string& data) noexcept;
    
    /**
     * Retreives message from the connected server.
     * @return Retrieved message.
     */
    std::string receive(unsigned int size) noexcept;
    
    /**
     * Connects to a server.
     * @return Whether the connection was succesful or not.
     */
    bool connect(const std::string& host, unsigned short port) noexcept;
    
    /**
     * Disconnects from the currently connected server.
     * @return Whether the disconnection was succesfull or not.
     * @return False if not connected to any server.
     */
    bool disconnect() noexcept;
    
    /**
     * Sets send and receive timeouts.
     */
    void setTimeout(long seconds, long microseconds) noexcept;
    
    /**
     * Sets send and receive timeouts.
     */
    void setTimeout(long seconds) noexcept;
    
    /**
     * Gets the server IP address.
     */
    std::string getRemoteAddress() const noexcept;
    
    /**
     * Gets the local port in which all communication currently runs though.
     */
    unsigned short getLocalPort() const noexcept;
    
    int handle;

protected:
    /**
     * Creates c-style address representation from a standard IP address and port.
     */
    sockaddr_in
    createAddress(const std::string& host, unsigned short port) const noexcept;

private:
    int domainToInt(Domain domain);
    int typeToInt(Type type);
    int protocolToInt(Protocol protocol);
};

} // namespace rokunet

