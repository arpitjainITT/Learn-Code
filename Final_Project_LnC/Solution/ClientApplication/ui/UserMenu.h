#ifndef USERMENU_H
#define USERMENU_H

#include "../models/User.h"

class UserMenu {
public:
    explicit UserMenu(const User& user);
    void display();

private:
    User currentUser;

    void showHeadlinesMenu();
    void showSavedArticlesMenu();
    void showSearchMenu();
    void showNotificationMenu();
};

#endif 
