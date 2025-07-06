#ifndef NOTIFICATIONMENU_H
#define NOTIFICATIONMENU_H

#include "../models/User.h"

class NotificationMenu {
public:
    explicit NotificationMenu(const User& user);
    void display();

private:
    User currentUser;

    void viewDeliveredNotifications();     // View actual notifications
    void viewPreferences();                // View category + keyword settings
    void configureCategory();
    void configureKeyword();
};

#endif // NOTIFICATIONMENU_H
