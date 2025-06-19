#include "Validator.h"
#include <regex>

bool Validator::isValidEmail(const std::string& email) {
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return std::regex_match(email, pattern);
}

bool Validator::isValidPassword(const std::string& password) {
    return password.length() >= 6;
}

bool Validator::isValidUsername(const std::string& username) {
    return !username.empty();
}
