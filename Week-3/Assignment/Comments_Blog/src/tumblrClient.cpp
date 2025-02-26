#include "TumblrClient.hpp"
#include <curl/curl.h>
#include <stdexcept>

TumblrClient::TumblrClient(const std::string& blogName) : blogName(blogName) {}

std::string TumblrClient::constructApiUrl(int start, int numPosts) const {
    return "https://" + blogName + ".tumblr.com/api/read/json?type=photo&start=" +
           std::to_string(start) + "&num=" + std::to_string(numPosts);
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

std::string TumblrClient::fetchPosts(int start, int numPosts) const {
    std::string apiUrl = constructApiUrl(start, numPosts);

    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize cURL.");
    }

    std::string responseString;
    curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("cURL request failed: " + std::string(curl_easy_strerror(res)));
    }

    curl_easy_cleanup(curl);
    return responseString;
}
