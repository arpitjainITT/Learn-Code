#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "../models/User.h"
#include <string>

class AuthService {
public:
    static bool login(const std::string& email, const std::string& password, User& user);
    static bool signup(const std::string& username, const std::string& email, const std::string& password);
};

#endif 
