#pragma once
#include "../models/User.h"

class NotificationMenu {
private:
    User user;
public:
    NotificationMenu(const User& u);
    void show();
};
