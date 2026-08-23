#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <string>

class HttpClient {
public:
    static std::string get(const std::string& url);
};

#endif