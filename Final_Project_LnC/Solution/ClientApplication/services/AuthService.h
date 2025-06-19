#pragma once
#include "../models/User.h"

class AuthService {
public:
    bool login(User& user);
    void signup();
};
