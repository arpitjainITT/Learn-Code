#ifndef IHTTPCLIENT_H
#define IHTTPCLIENT_H

#include <string>

class IHttpClient {
public:
    virtual std::string get(const std::string& url) = 0;
    virtual ~IHttpClient() = default;
};

#endif
