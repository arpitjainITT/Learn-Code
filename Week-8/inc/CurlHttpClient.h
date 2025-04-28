#ifndef CurlHttpClient_H
#define CurlHttpClient_H

#include "IHttpClient.h"
#include <curl/curl.h>

class CurlHttpClient : public IHttpClient {
public:
    std::string get(const std::string& url) override;
};


#endif