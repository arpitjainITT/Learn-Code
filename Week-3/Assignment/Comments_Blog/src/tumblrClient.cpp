#include "TumblrClient.hpp"
#include <curl/curl.h>
#include <stdexcept>

TumblrClient::TumblrClient(const std::string& blogName) : blogName(blogName) {}

std::string TumblrClient::constructApiUrl(int start, int totalPosts) const {
    return cWebUrlProtocol + blogName + cWebUrlDomainName + cWebUrlResourcePath + 
           cWebUrlTypeQueryParameter + cWebUrlStartingIndexQueryParameter +
           std::to_string(start) + cWebUrlTotalPostsQueryParameter + std::to_string(totalPosts);
}

size_t TumblrClient::writeCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append(static_cast<char*>(contents), newLength);
    } catch (const std::bad_alloc&) {
        return 0;
    }
    return newLength;
}

std::string TumblrClient::fetchPosts(int start, int totalPosts) const {
    std::string apiUrl = constructApiUrl(start, totalPosts);

    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error(cFailedToIntializeCURL);
    }

    std::string responseString;
    curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error(cCURLRequestFailed + std::string(curl_easy_strerror(res)));
    }

    curl_easy_cleanup(curl);
    return responseString;
}
