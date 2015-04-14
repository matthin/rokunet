#include "Catch.h"
#include <Http/Response.h>

TEST_CASE("Factory builds object") {
    const auto response = rokunet::Http::Response::Factory(
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 8\r\n"
        "\r\n"
        "test\ning"
        "\r\n"
    ).build();
    
    REQUIRE(response.code == 200);
    REQUIRE(response.codeMessage == "OK");
    REQUIRE(response.headers.size() == 1);
    REQUIRE(response.headers.at("Content-Length") == "8");
    REQUIRE(response.body == "test\ning");
}

