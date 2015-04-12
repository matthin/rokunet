#include "Catch.h"
#include <Http/Request.h>

TEST_CASE("Prepares") {
    const auto request = rokunet::Http::Request::Builder()
                         .setHost("google.com")
                         .setLocation("/index.html")
                         .setMethod(rokunet::Http::Method::Put)
                         .setVersion(rokunet::Http::Version(1, 2))
                         .build();
    REQUIRE(request.prepare() == "PUT /index.html HTTP/1.2\r\n");
}

