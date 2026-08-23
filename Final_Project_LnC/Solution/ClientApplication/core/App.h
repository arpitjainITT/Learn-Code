#pragma once
#include <string>
#include "../models/User.h"

// Forward declaration of test fixture classes
class AppTest;
class AppTest_ShowHomeMenu_DisplaysCorrectStrings_Test;
class AppTest_HandleLogin_InvalidCredentials_ShowsLoginFailed_Test;
class AppTest_HandleSignup_SuccessAndFailure_Test;
class AppTest_ShowAdminMenu_CallsDisplay_Test;
class AppTest_ShowUserMenu_CallsDisplay_Test;

class App {
public:
    void run();

protected:  
    void showHomeMenu();
    void handleLogin();
    void handleSignup();

    void showAdminMenu(const User& user);
    void showUserMenu(const User& user);

    int promptUserIdFromAuth(const std::string& email, const std::string& password);
};
