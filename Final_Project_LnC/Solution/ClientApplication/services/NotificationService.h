#pragma once
#include "../models/User.h"

class NotificationService {
public:
    static void viewNotifications(const User& user);
    static void configureNotifications(const User& user);
};
