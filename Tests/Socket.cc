#include "Catch.h"
#include <Socket.h>

// Maybe should be mocked
#include <Listener.h>

TEST_CASE("Communicates") {
    rokunet::Listener server;
    server.listen(8080);

    rokunet::Socket client;
    client.connect("127.0.0.1", 8080);

    rokunet::Socket socket;
    server.accept(&socket);

    std::string message("Testing123");
    client.send(message);

    const auto response = socket.receive(message.size());
    REQUIRE(response == message);
}

