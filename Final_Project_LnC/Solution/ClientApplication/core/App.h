#pragma once
#include <string>
#include "../models/User.h"

class App {
public:
    void run();

private:
    void showHomeMenu();
    void handleLogin();
    void handleSignup();

    void showAdminMenu(const User& user);
    void showUserMenu(const User& user);

    int promptUserIdFromAuth(const std::string& email, const std::string& password);
};
