#include "Catch.h"
#include <Socket.h>

// Maybe should be mocked
#include <Listener.h>

#include <chrono>
#include <thread>

TEST_CASE("Communicates") {
    rokunet::Listener server;
    server.listen(8082);

    rokunet::Socket client;
    client.connect("127.0.0.1", 8082);

    rokunet::Socket socket;
    server.accept(&socket);

    std::string message("Testing123");
    client.send(message);

    const auto response = socket.receive(message.size());
    REQUIRE(response == message);
}

TEST_CASE("connects") {
    // Since this is a one time very quick unit test
    // we don't need to care about freeing the memory.
    auto serverReady = new bool(false);
    std::thread serverThread([serverReady]() mutable {
        rokunet::Listener server;
        server.listen(8083);

        *serverReady = true;

        rokunet::Socket client;
        server.accept(&client);
    });
    while (!*serverReady) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    };

    rokunet::Socket client;
    REQUIRE(client.connect("127.0.0.1", 8083));

    serverThread.join();
}

TEST_CASE("disconnects") {
    rokunet::Listener server;
    server.listen(8084);

    rokunet::Socket client;
    client.connect("127.0.0.1", 8084);

    rokunet::Socket socket;
    server.accept(&socket);

    REQUIRE(client.disconnect());
}

