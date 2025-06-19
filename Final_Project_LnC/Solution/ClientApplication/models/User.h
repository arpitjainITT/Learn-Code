#pragma once
#include <string>

class User {
private:
    std::string username;
    std::string email;
    std::string role;
    std::string token;

public:
    void setUsername(const std::string& name) { username = name; }
    void setEmail(const std::string& mail) { email = mail; }
    void setRole(const std::string& r) { role = r; }
    void setToken(const std::string& t) { token = t; }

    std::string getUsername() const { return username; }
    std::string getEmail() const { return email; }
    std::string getRole() const { return role; }
    std::string getToken() const { return token; }
};
