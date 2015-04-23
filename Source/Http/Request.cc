#include "Http/Request.h"

#include <sstream>
#include <memory>
#include <unordered_map>

namespace rokunet {
namespace Http {

std::string Request::methodToString(const Method method) {
    const std::string temp[] = {
        "DELETE",
        "GET",
        "HEAD",
        "POST",
        "PUT"
    };

    return std::string(
        temp[static_cast<int>(method)]
    );
}

Request::Method Request::stringToMethod(const std::string& textForm) {
    const std::unordered_map<std::string, unsigned short> map {
        {"DELETE", 0},
        {"GET", 1},
        {"HEAD", 2},
        {"POST", 3},
        {"PUT", 4}
    };

    return static_cast<Method>(map.at(textForm));
}

Request::Request(const std::string body,
                 const std::unordered_map<std::string, std::string> headers,
                 const std::string host,
                 const std::string location,
                 const Method method,
                 const Version version)
    : body(body), headers(headers), host(host), location(location),
    method(method), version(version) {
}

Response Request::send() const {
    std::unique_ptr<Socket> socket(new Socket);
    return send(socket.get());
}

Response Request::send(Socket* socket) const {
    socket->connect(host, 80);
    socket->send(prepare());

    std::ostringstream rawResponse;
    for (int i = 0; i < 10; ++i) {
        rawResponse << socket->receive(256);
        if (rawResponse.str().size() / i != 256) {
            break;
        }
    }

    return Response::Factory(rawResponse.str()).build();
}

std::string Request::prepare() const {
    std::ostringstream out;

    out << methodToString(method);

    out << ' ' << location;
    out << " HTTP/" << version.major << '.' << version.minor << "\r\n";

    for (const auto header : headers) {
        out << header.first << ": " << header.second << "\r\n";
    }

    if (body.size() > 0) {
        out << "\r\n";
        out << body;
    }

    return out.str();
}

} // namespace Http
} // namespace rokunet

