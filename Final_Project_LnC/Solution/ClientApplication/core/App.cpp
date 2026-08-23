#include "App.h"
#include "../ui/LoginMenu.h"
#include "../ui/UserMenu.h"
#include "../ui/AdminMenu.h"
#include "../services/AuthService.h"
#include "../utils/ConsoleUtils.h"
#include "../constants/Strings.h"

#include <iostream>
#include <string>

void App::run() {
    bool running = true;
    while (running) {
        showHomeMenu();
        int choice = ConsoleUtils::getValidatedInput(1, 3);
        switch (choice) {
            case 1:
                handleLogin();
                break;
            case 2:
                handleSignup();
                break;
            case 3:
                std::cout << Strings::APP_EXITING;
                running = false;
                break;
            default:
                std::cout << Strings::APP_INVALID_CHOICE;
        }
    }
}

void App::showHomeMenu() {
    // ConsoleUtils::clear();
    std::cout << Strings::APP_HOME_SEPARATOR;
    std::cout << Strings::APP_HOME_WELCOME;
    std::cout << Strings::APP_HOME_SEPARATOR;
    std::cout << Strings::APP_HOME_LOGIN;
    std::cout << Strings::APP_HOME_SIGNUP;
    std::cout << Strings::APP_HOME_EXIT;
    std::cout << Strings::APP_HOME_ENTER_CHOICE;
}

void App::handleLogin() {
    std::string email, password;
    std::cout << Strings::APP_LOGIN_EMAIL;
    std::cin >> email;
    std::cout << Strings::APP_LOGIN_PASSWORD;
    std::cin >> password;

    User user;
    if (AuthService::login(email, password, user)) {
        if (user.getRole() == "admin") {
            showAdminMenu(user);
        } else {
            showUserMenu(user);
        }
    } else {
        std::cout << Strings::APP_LOGIN_FAILED;
    }
}

void App::handleSignup() {
    std::string username, email, password;
    std::cout << Strings::APP_SIGNUP_USERNAME;
    std::cin >> username;
    std::cout << Strings::APP_SIGNUP_EMAIL;
    std::cin >> email;
    std::cout << Strings::APP_SIGNUP_PASSWORD;
    std::cin >> password;

    if (AuthService::signup(username, email, password)) {
        std::cout << Strings::APP_SIGNUP_SUCCESS;
    } else {
        std::cout << Strings::APP_SIGNUP_FAILED;
    }
}

void App::showAdminMenu(const User& user) {
    AdminMenu menu(user);
    menu.display();
}

void App::showUserMenu(const User& user) {
    UserMenu menu(user);
    menu.display();
}
