#pragma once
#include "../models/User.h"

class AdminMenu {
private:
    User user;
public:
    AdminMenu(const User& u);
    void show();
};
