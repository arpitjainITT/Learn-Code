#include "LoginMenu.h"
#include "UserMenu.h"
#include "AdminMenu.h"
#include "../services/AuthService.h"
#include "../utils/ConsoleUtils.h"
#include "../models/User.h"
#include <iostream>

void LoginMenu::show() {
    int choice;
    do {
        std::cout << "=== News Aggregator ===\n";
        std::cout << "1. Login\n2. Sign Up\n3. Exit\nChoice: ";
        std::cin >> choice;

        AuthService auth;
        User user;

        switch (choice) {
            case 1:
                if (auth.login(user)) {
                    if (user.getRole() == "admin") AdminMenu(user).show();
                    else UserMenu(user).show();
                }
                break;
            case 2:
                auth.signup();
                break;
            case 3:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (choice != 3);
}
