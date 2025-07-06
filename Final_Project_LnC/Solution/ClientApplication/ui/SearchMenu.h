#ifndef SEARCHMENU_H
#define SEARCHMENU_H

#include "../models/User.h"

class SearchMenu {
public:
    explicit SearchMenu(const User& user);
    void display();

private:
    User currentUser;

    void performSearch();
};

#endif 
