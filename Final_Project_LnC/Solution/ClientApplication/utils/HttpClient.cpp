#include "HttpClient.h"
#include <iostream>
#include <string>

static std::string authToken = "";

void HttpClient::setAuthToken(const std::string& token) {
    authToken = token;
}

std::string HttpClient::get(const std::string& endpoint) {
    std::cout << "[GET] " << endpoint << "\nAuth: Bearer " << authToken << "\n";
    return "{}";
}

std::string HttpClient::post(const std::string& endpoint, const std::string& body) {
    std::cout << "[POST] " << endpoint << "\nBody: " << body << "\n";
    return "{\"success\": true}";
}

std::string HttpClient::put(const std::string& endpoint, const std::string& body) {
    std::cout << "[PUT] " << endpoint << "\nBody: " << body << "\n";
    return "{\"success\": true}";
}

std::string HttpClient::deleteRequest(const std::string& endpoint) {
    std::cout << "[DELETE] " << endpoint << "\n";
    return "{\"success\": true}";
}
