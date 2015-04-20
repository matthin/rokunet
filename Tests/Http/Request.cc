#include "Catch.h"
#include <Http/Request.h>

TEST_CASE("Prepares") {
    const auto request = rokunet::Http::Request::Builder()
                         .setHost("google.com")
                         .setLocation("/index.html")
                         .setMethod(rokunet::Http::Request::Method::Put)
                         .setVersion(rokunet::Http::Version(1, 2))
                         .build();
    REQUIRE(request.prepare() == "PUT /index.html HTTP/1.2\r\n");
}

TEST_CASE("Factory builds Request") {
    const auto request = rokunet::Http::Request::Factory(
        "GET /index.html HTTP/1.1\r\n"
        "Accept: text/plain\r\n"
        "\r\n"
    ).build();

    REQUIRE(request.version == rokunet::Http::Version(1, 1));
    REQUIRE(request.headers.at("Accept") == "text/plain");
}

