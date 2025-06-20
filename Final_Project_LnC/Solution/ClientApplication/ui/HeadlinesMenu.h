#pragma once

#include "../models/User.h"

class HeadlinesMenu {
private:
    User user;
public:
    HeadlinesMenu(const User& user);
    void show();
};
