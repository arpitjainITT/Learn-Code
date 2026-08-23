#ifndef USER_H
#define USER_H

#include <string>

class User {
    int         id;
    std::string role;
    std::string email;
    std::string token;  // JWT Bearer token — set after successful login

public:
    User() : id(-1), role("user"), email(""), token("") {}

    int         getId()    const { return id;    }
    std::string getRole()  const { return role;  }
    std::string getEmail() const { return email; }
    std::string getToken() const { return token; }

    void setId(int userId)              { id    = userId; }
    void setRole(const std::string& r)  { role  = r; }
    void setEmail(const std::string& e) { email = e; }
    void setToken(const std::string& t) { token = t; }
};

#endif 
