#include "Http/Response.h"

#include <memory>

namespace rokunet {
namespace Http {

Response::Response(
    const unsigned short code,
    const std::string codeMessage,
    const std::unordered_map<std::string, std::string>& headers,
    const std::string body
) : code(code), codeMessage(codeMessage), headers(headers), body(body) {}

std::string Response::prepare() const {
    std::ostringstream out;

    out << "HTTP/1.1 " << code << ' ' << codeMessage << "\r\n";

    for (const auto header : headers) {
        out << header.first << ": " << header.second << "\r\n";
    }

    if (body.size() > 0) {
        out << "\r\n";
        out << body;
    }

    return out.str();
}

} // Http
} // rokunet

