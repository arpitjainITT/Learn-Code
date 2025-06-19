#pragma once

#include "../models/User.h"

class HeadlinesMenu {
private:
    User user;
public:
    HeadlinesMenu(const User& u);
    void show();
};
