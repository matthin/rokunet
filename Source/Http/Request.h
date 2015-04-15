#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <memory>
#include "Response.h"
#include "Socket.h"

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
    bool operator ==(const Version& other) const {
        return major == other.major && minor == other.minor;
    }
    bool operator !=(const Version& other) const {
        return !(major == other.major && minor == other.minor);
    }
    const unsigned short major;
    const unsigned short minor;
};

class Request {
public:
    class Builder;
    class Factory;

    Request(
        std::string body,
        std::unordered_map<std::string, std::string> headers,
        std::string host,
        std::string location,
        Method method,
        Version version
    );

    Response send() const;
    Response send(Socket* socket) const;
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

class Request::Factory {
public:
    Factory(const std::string& rawRequest) {
        std::istringstream stream(rawRequest);

        std::string methodText;
        std::getline(stream, methodText, ' ');
        if (methodText == "DELETE") {
            method = Method::Delete;
        } else if (methodText == "GET") {
            method = Method::Get;
        } else if (methodText == "HEAD") {
            method = Method::Head;
        } else if (methodText == "POST") {
            method = Method::Post;
        } else if (methodText == "PUT") {
            method = Method::Put;
        }

        std::string location;
        std::getline(stream, location, ' ');

        std::string protocol;
        std::getline(stream, protocol, '\n');

        std::istringstream protocolStream(protocol);
        // Only leave protocol version
        protocolStream.ignore(5, '/');

        std::string major;
        std::getline(protocolStream, major, '.');

        std::string minor;
        std::getline(protocolStream, minor, '\r');

        version.reset(
            new Version(
                stoi(major), stoi(minor)
            )
        );

        // Parse headers.
        while (stream.peek() != '\r') {
            std::string rawHeader;
            std::getline(stream, rawHeader, '\n');

            std::istringstream headerStream(rawHeader);

            std::string key;
            std::getline(headerStream, key, ':');

            std::string value;
            std::getline(headerStream, value, '\r');
            // Don't need leading space char.
            value.erase(0, 1);

            headers[key] = value;
        }
    }

    Request build() const {
        return Request(body, headers, host, location, method, *version);
    }
private:
    std::string body;
    std::unordered_map<std::string, std::string> headers;
    std::string location;
    std::string host;
    std::unique_ptr<Version> version;
    Method method;
};

} // namespace Http
} // namespace rokunet

