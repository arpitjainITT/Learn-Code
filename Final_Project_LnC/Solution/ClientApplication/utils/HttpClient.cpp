#include "HttpClient.h"
#include "../constants/Strings.h"
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <sstream>

static std::string authToken = "";

void HttpClient::setAuthToken(const std::string& token) {
    authToken = token;
}

// CURL write callback
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Private reusable internal function
static std::string sendRequest(const std::string& url, const std::string& method, const std::string& body = "") {
    std::cout << Strings::HTTP_LOG_METHOD_URL;
    std::cout << "[" << method << "] " << url << "\n";
    if (!body.empty()) {
        std::cout << Strings::HTTP_LOG_BODY;
        std::cout << "Body: " << body << "\n";
    }
    if (!authToken.empty()) {
        std::cout << Strings::HTTP_LOG_AUTH;
        std::cout << "Auth: Bearer " << authToken << "\n";
    }

    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        struct curl_slist* headers = NULL;
        if (!authToken.empty()) {
            std::string bearer = Strings::HTTP_HEADER_AUTH_BEARER + authToken;
            headers = curl_slist_append(headers, bearer.c_str());
        }

        if (method == Strings::HTTP_METHOD_POST) {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            headers = curl_slist_append(headers, Strings::HTTP_HEADER_CONTENT_TYPE_JSON.c_str());
        } else if (method == Strings::HTTP_METHOD_PUT) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, Strings::HTTP_METHOD_PUT.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            headers = curl_slist_append(headers, Strings::HTTP_HEADER_CONTENT_TYPE_JSON.c_str());
        } else if (method == Strings::HTTP_METHOD_DELETE) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, Strings::HTTP_METHOD_DELETE.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << Strings::HTTP_ERROR_REQUEST_FAILED;
            std::cerr << method << " request failed: " << curl_easy_strerror(res) << "\n";
        }

        if (headers) curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        std::cerr << Strings::HTTP_ERROR_CURL_INIT;
        std::cerr << "Failed to initialize CURL\n";
    }

    return response;
}

std::string HttpClient::get(const std::string& endpoint) {
    return sendRequest(endpoint, Strings::HTTP_METHOD_GET);
}

std::string HttpClient::post(const std::string& endpoint, const std::string& body) {
    return sendRequest(endpoint, Strings::HTTP_METHOD_POST, body);
}

std::string HttpClient::put(const std::string& endpoint, const std::string& body) {
    return sendRequest(endpoint, Strings::HTTP_METHOD_PUT, body);
}

std::string HttpClient::deleteRequest(const std::string& endpoint) {
    return sendRequest(endpoint, Strings::HTTP_METHOD_DELETE);
}
