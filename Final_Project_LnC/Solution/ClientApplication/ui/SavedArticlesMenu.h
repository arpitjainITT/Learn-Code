#pragma once
#include "../models/User.h"

class SavedArticlesMenu {
private:
    User user;
public:
    SavedArticlesMenu(const User& u);
    void show();
};
