#include "Http/Request.h"

#include <sstream>
#include <memory>

namespace rokunet {
namespace Http {

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

    switch (method) {
    case Method::Delete:
        out << "DELETE";
        break;
    case Method::Get:
        out << "GET";
        break;
    case Method::Head:
        out << "HEAD";
        break;
    case Method::Post:
        out << "POST";
        break;
    case Method::Put:
        out << "PUT";
        break;
    }

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

