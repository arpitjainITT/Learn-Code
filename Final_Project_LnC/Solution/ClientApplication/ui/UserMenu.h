#pragma once

#include "../models/User.h"

class UserMenu {
private:
    User user;
public:
    UserMenu(const User& u);
    void show();
};
