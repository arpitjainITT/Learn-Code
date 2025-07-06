#include "Validator.h"
#include "../constants/Strings.h"
#include <regex>

bool Validator::isValidEmail(const std::string& email) {
    const std::regex pattern(Strings::VALIDATOR_EMAIL_REGEX);
    return std::regex_match(email, pattern);
}

bool Validator::isValidPassword(const std::string& password) {
    return password.length() >= Strings::VALIDATOR_PASSWORD_MIN_LENGTH;
}

bool Validator::isValidUsername(const std::string& username) {
    return !username.empty();
}
