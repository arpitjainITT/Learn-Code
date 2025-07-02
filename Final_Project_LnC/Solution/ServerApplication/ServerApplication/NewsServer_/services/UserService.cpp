#include "UserService.hpp"
#include "../database/Database.hpp"
#include <iostream>

using json = nlohmann::json;

bool UserService::login(const std::string& email, const std::string& password, int& userId, std::string& role) {
    return Database::authenticateUser(email, password, userId, role);
}

bool UserService::signup(const std::string& username, const std::string& email, const std::string& password) {
    return Database::registerUser(username, email, password);
}

int UserService::getUserIdByEmail(const std::string& email) {
    return Database::getUserIdByEmail(email);
}
