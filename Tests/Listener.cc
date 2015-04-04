#include "Catch.h"
#include <Listener.h>

TEST_CASE("Listens on port") {
    rokunet::Listener listener;
    listener.listen(8080);
    REQUIRE(listener.getLocalPort() == 8080);
}

TEST_CASE("Accepts new clients") {
    rokunet::Listener listener;
    listener.listen(8081);

    rokunet::Socket socket;
    socket.connect("127.0.0.1", 8081);

    listener.accept(&socket);

    REQUIRE(socket.getLocalPort() != 0);
}

