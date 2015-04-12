#include "Http/Request.h"

#include "Socket.h"
#include <sstream>

namespace rokunet {
namespace Http {

Request::Request(const std::string body,
                 const std::unordered_map<std::string, std::string> headers,
                 const std::string host,
                 const std::string location,
                 const Method method,
                 const Version version)
    : body(body), headers(headers), location(location), method(method),
    version(version) {
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

