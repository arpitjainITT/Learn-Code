#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include <string>

class HttpClient {
public:
    static std::string get(const std::string& endpoint);
    static std::string post(const std::string& endpoint, const std::string& body);
    static std::string put(const std::string& endpoint, const std::string& body);
    static std::string deleteRequest(const std::string& endpoint);
    static void setAuthToken(const std::string& token);

};

#endif