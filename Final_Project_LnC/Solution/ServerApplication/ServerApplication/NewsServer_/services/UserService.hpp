#ifndef USERSERVICE_HPP
#define USERSERVICE_HPP

#include <nlohmann/json.hpp>
#include <string>

class UserService {
public:
    static bool login(const std::string& email, const std::string& password, int& userId, std::string& role);
    static bool signup(const std::string& username, const std::string& email, const std::string& password);
    static int getUserIdByEmail(const std::string& email);
};

#endif
