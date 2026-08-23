#include "LoginMenu.h"
#include "UserMenu.h"
#include "AdminMenu.h"
#include "../services/AuthService.h"
#include "../utils/ConsoleUtils.h"
#include "../models/User.h"
#include "../utils/Validator.h"
#include "../constants/Strings.h"
#include <iostream>

void LoginMenu::show() {
    int choice;
    do {
        // ConsoleUtils::clear();
        std::cout << Strings::LOGIN_MENU_TITLE;
        std::cout << Strings::LOGIN_MENU_LOGIN;
        std::cout << Strings::LOGIN_MENU_SIGNUP;
        std::cout << Strings::LOGIN_MENU_EXIT;
        std::cout << Strings::LOGIN_MENU_CHOICE;
        std::cin >> choice;

        std::string email, password, username;
        User user;

        switch (choice) {
            case 1:
                std::cout << Strings::LOGIN_MENU_ENTER_EMAIL;
                std::cin >> email;
                std::cout << Strings::LOGIN_MENU_ENTER_PASSWORD;
                std::cin >> password;

                if (AuthService::login(email, password, user)) {
                    if (user.getRole() == "admin") AdminMenu(user).display();
                    else UserMenu(user).display();
                } else {
                    std::cout << Strings::LOGIN_MENU_LOGIN_FAILED;
                }
                break;

            case 2:
                std::cout << Strings::LOGIN_MENU_ENTER_USERNAME;
                std::cin >> username;
                std::cout << Strings::LOGIN_MENU_ENTER_EMAIL;
                std::cin >> email;
                std::cout << Strings::LOGIN_MENU_ENTER_PASSWORD;
                std::cin >> password;

                if (!Validator::isValidEmail(email)) {
                    std::cout << Strings::LOGIN_MENU_INVALID_EMAIL;
                    break;
                }
                if (!Validator::isValidPassword(password)) {
                    std::cout << Strings::LOGIN_MENU_INVALID_PASSWORD;
                    break;
                }

                if (AuthService::signup(username, email, password)) {
                    std::cout << Strings::LOGIN_MENU_SIGNUP_SUCCESS;
                } else {
                    std::cout << Strings::LOGIN_MENU_SIGNUP_FAILED;
                }
                break;

            case 3:
                std::cout << Strings::LOGIN_MENU_EXITING;
                break;

            default:
                std::cout << Strings::LOGIN_MENU_INVALID_CHOICE;
        }

        ConsoleUtils::pause();

    } while (choice != 3);
}
