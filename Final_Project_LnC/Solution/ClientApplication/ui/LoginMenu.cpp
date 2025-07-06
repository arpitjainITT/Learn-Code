#include "LoginMenu.h"
#include "UserMenu.h"
#include "AdminMenu.h"
#include "../services/AuthService.h"
#include "../utils/ConsoleUtils.h"
#include "../models/User.h"
#include "../utils/Validator.h"
#include <iostream>

void LoginMenu::show() {
    int choice;
    do {
        // ConsoleUtils::clear();
        std::cout << "=== News Aggregator ===\n";
        std::cout << "1. Login\n2. Sign Up\n3. Exit\nChoice: ";
        std::cin >> choice;

        std::string email, password, username;
        User user;

        switch (choice) {
            case 1:
                std::cout << "Enter email: ";
                std::cin >> email;
                std::cout << "Enter password: ";
                std::cin >> password;

                if (AuthService::login(email, password, user)) {
                    if (user.getRole() == "admin") AdminMenu(user).display();
                    else UserMenu(user).display();
                } else {
                    std::cout << "Login failed.\n";
                }
                break;

            case 2:
                std::cout << "Enter username: ";
                std::cin >> username;
                std::cout << "Enter email: ";
                std::cin >> email;
                std::cout << "Enter password: ";
                std::cin >> password;

                if (!Validator::isValidEmail(email)) {
                    std::cout << "Invalid email format.\n";
                    break;
                }
                if (!Validator::isValidPassword(password)) {
                    std::cout << "Password must be at least 6 characters.\n";
                    break;
                }

                if (AuthService::signup(username, email, password)) {
                    std::cout << "Signup successful! Please login.\n";
                } else {
                    std::cout << "Signup failed.\n";
                }
                break;

            case 3:
                std::cout << "Exiting...\n";
                break;

            default:
                std::cout << "Invalid choice.\n";
        }

        ConsoleUtils::pause();

    } while (choice != 3);
}
