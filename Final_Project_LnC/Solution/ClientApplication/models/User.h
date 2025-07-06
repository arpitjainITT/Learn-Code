#ifndef USER_H
#define USER_H

#include <string>

class User {
    int id;
    std::string role;
    std::string email;

public:
    User() : id(-1), role("user"), email("") {}

    int getId() const { return id; }
    std::string getRole() const { return role; }
    std::string getEmail() const { return email; }

    void setId(int userId) { id = userId; }
    void setRole(const std::string& r) { role = r; }
    void setEmail(const std::string& e) { email = e; }
};

#endif 
