#pragma once
#include "../models/User.h"

class SearchMenu {
private:
    User user;
public:
    SearchMenu(const User& u);
    void show();
};
