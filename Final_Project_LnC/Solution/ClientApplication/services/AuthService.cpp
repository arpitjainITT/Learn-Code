#include "AuthService.h"
#include "../utils/HttpClient.h"
#include "../utils/Validator.h"
#include "../utils/ConsoleUtils.h"
#include "../constants/APIEndpoints.h"
#include <iostream>

bool AuthService::login(User& user) {
    std::string email, password;
    std::cout << "Email: "; std::cin >> email;
    std::cout << "Password: "; std::cin >> password;

    std::string body = "{\"email\":\"" + email + "\",\"password\":\"" + password + "\"}";
    std::string response = HttpClient::post(std::string(BASE_URL) + LOGIN_ENDPOINT, body);

    if (response.find("success") != std::string::npos) {
        user.setEmail(email);
        user.setUsername("Suresh"); // ideally parsed from JSON
        user.setRole(response.find("admin") != std::string::npos ? "admin" : "user");
        user.setToken("mocked_token_123"); // replace with parsed value in real

        return true;
    }

    std::cout << "Login failed.\n";
    return false;
}

void AuthService::signup() {
    std::string name, email, password;
    std::cout << "Username: "; std::cin >> name;
    std::cout << "Email: "; std::cin >> email;
    std::cout << "Password: "; std::cin >> password;

    if (!Validator::isValidEmail(email)) {
        std::cout << "Invalid email.\n";
        return;
    }

    std::string body = "{\"username\":\"" + name + "\",\"email\":\"" + email + "\",\"password\":\"" + password + "\"}";
    std::string response = HttpClient::post(std::string(BASE_URL) + SIGNUP_ENDPOINT, body);

    std::cout << (response.find("success") != std::string::npos ? "Signup successful.\n" : "Signup failed.\n");
}
