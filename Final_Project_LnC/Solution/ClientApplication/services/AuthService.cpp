#include "AuthService.h"
#include "../utils/HttpClient.h"
#include "../constants/APIEndpoints.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

bool AuthService::login(const std::string& email, const std::string& password, User& user) {
    json requestBody = {
        {"email", email},
        {"password", password}
    };

    std::string responseStr = HttpClient::post(API::LOGIN, requestBody.dump());
    json response;
    try {
        response = json::parse(responseStr);
        if (response["status"] == "success") {
            user.setId(response["user_id"]);
            user.setRole(response["role"]);
            user.setEmail(email); // email is known already from input
            
            return true;
        }        
    } catch (...) {
        std::cerr << "Login failed. Response: " << responseStr << "\n";
    }

    return false;
}

bool AuthService::signup(const std::string& username, const std::string& email, const std::string& password) {
    json requestBody = {
        {"username", username},
        {"email", email},
        {"password", password}
    };

    std::string responseStr = HttpClient::post(API::SIGNUP, requestBody.dump());
    json response;
    try {
        response = json::parse(responseStr);
        return response["status"] == "user registered";
    } catch (...) {
        std::cerr << "Signup failed. Response: " << responseStr << "\n";
        return false;
    }
}
