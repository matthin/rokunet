#include "Response.h"

namespace rokunet {
namespace Http {

Response::Response(
    const unsigned short code,
    const std::string codeMessage,
    const std::unordered_map<std::string, std::string>& headers,
    const std::string body
) : code(code), codeMessage(codeMessage), headers(headers), body(body) {}

} // Http
} // rukunet

