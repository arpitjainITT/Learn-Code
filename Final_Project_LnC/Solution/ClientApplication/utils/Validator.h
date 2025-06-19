#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

class Validator {
public:
    static bool isValidEmail(const std::string& email);
    static bool isValidPassword(const std::string& password);
    static bool isValidUsername(const std::string& username);
};

#endif