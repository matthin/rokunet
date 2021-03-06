#pragma once

#include <unordered_map>
#include <sstream>
#include <string>

namespace rokunet {
namespace Http {

class Response {
public:
    class Builder;
    class Factory; 

    Response(
        unsigned short code,
        std::string codeMessage,
        const std::unordered_map<std::string, std::string>& headers,
        std::string body
    );

    std::string prepare() const;

    const unsigned short code;
    const std::string codeMessage;
    const std::unordered_map<std::string, std::string> headers;
    const std::string body;
};

class Response::Builder {
public:
    Builder& setCode(const unsigned short code) {
        this->code = code;
        return *this;
    }
    Builder& setCodeMessage(const std::string codeMessage) {
        this->codeMessage = codeMessage;
        return *this;
    }
    Builder& setHeaders(const std::unordered_map<std::string,
                                                 std::string> headers) {
        this->headers = headers;
        return *this;
    }
    Builder& setBody(const std::string body) {
        this->body = body;
        return *this;
    }

    Response build() const {
        return Response(code, codeMessage, headers, body);
    }

private:
    unsigned short code;
    std::string codeMessage;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class Response::Factory {
public:
    /**
     * Constructs a Response from its text representation.
     * Useful when running an HTTP client.
     */
    Factory(const std::string rawResponse) {
        std::istringstream stream(rawResponse);

        std::string protocol;
        std::getline(stream, protocol, ' ');

        std::string codeAsText;
        std::getline(stream, codeAsText, ' ');
        code = std::stoi(codeAsText);

        std::getline(stream, codeMessage, '\n');
        // Remove trailing \r.
        codeMessage.pop_back();

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

        // Body shouldn't exist unless Content-Length also does.
        if (headers.find("Content-Length") == headers.end()) {
            return;
        }

        // Remove leading \r\n
        stream.ignore(2, '\n');

        // Parse body.
        const int bodyLength = stoi(headers.at("Content-Length")); 
        char buffer[bodyLength];
        stream.read(buffer, bodyLength);
        body = buffer;
        // Not sure why, but string contains extra useless characters.
        body.resize(bodyLength);
    }

    Response build() const {
        return Response(code, codeMessage, headers, body);
    }

private:
    unsigned short code;
    std::string codeMessage;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

} // Http
} // rokunet

