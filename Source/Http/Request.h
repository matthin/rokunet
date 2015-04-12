#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace rokunet {
namespace Http {

enum struct Method {
    Delete,
    Get,
    Head,
    Post,
    Put,
};

// gnu macros major and minor exist for some dumb reason.
// See http://stackoverflow.com/questions/22240973/major-and-minor-macros-defined-in-sys-sysmacros-h-pulled-in-by-iterator
#undef major
#undef minor

struct Version {
    Version(const unsigned short major, const unsigned short minor)
        : major(major), minor(minor) {};
    Version(const Version& original)
        : major(original.major), minor(original.minor) {};
    const unsigned short major;
    const unsigned short minor;
};

class Request {
public:
    class Builder;

    Request(
        std::string body,
        std::unordered_map<std::string, std::string> headers,
        std::string host,
        std::string location,
        Method method,
        Version version
    );

    std::string prepare() const;

    const std::string body;
    const std::unordered_map<std::string, std::string> headers;
    const std::string host;
    const Method method;
    const std::string location;
    const Version version;
};

class Request::Builder {
public:
    Builder& setBody(const std::string body) {
        this->body = body;
        return *this;
    }
    Builder& setHeaders(const std::unordered_map<std::string,
                                                 std::string> headers) {
        this->headers = headers;
        return *this;
    }
    Builder& setHost(const std::string host) {
        this->host = host;
        return *this;
    }
    Builder& setLocation(const std::string location) {
        this->location = location;
        return *this;
    }
    Builder& setMethod(const Method method) {
        this->method = method;
        return *this;
    }
    Builder& setVersion(const Version version) {
        this->version.reset(new Version(version));
        return *this;
    }

    Request build() {
        return Request(body, headers, host, location, method, *version);
    }

private:
    std::string body;
    std::unordered_map<std::string, std::string> headers;
    std::string host = "127.0.0.1";
    std::string location = "/";
    Method method = Method::Get;
    // Needs to be on the heap due to a weird const copy assignment issue.
    std::unique_ptr<Version> version =
        std::unique_ptr<Version>(new Version(1, 1));
};

} // namespace Http
} // namespace rokunet

