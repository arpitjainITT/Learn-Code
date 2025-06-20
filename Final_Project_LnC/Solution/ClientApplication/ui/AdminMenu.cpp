#include "AdminMenu.h"
#include "../services/AdminService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>

AdminMenu::AdminMenu(const User& u) : user(u) {}

void AdminMenu::show() {
    int choice;
    do {
        // ConsoleUtils::clear();
        std::cout << "=== Admin Menu ===\n";
        std::cout << "1. View List of External Servers and Status\n";
        std::cout << "2. View External Server Details\n";
        std::cout << "3. Update/Edit External Server Details\n";
        std::cout << "4. Add New News Category\n";
        std::cout << "5. Logout\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                AdminService::viewServerStatus();
                break;
            case 2:
                AdminService::viewServerDetails();
                break;
            case 3:
                AdminService::updateServerDetails();
                break;
            case 4:
                AdminService::addCategory();
                break;
            case 5:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid option.\n";
        }

        if (choice != 5) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(); std::cin.get();
        }

    } while (choice != 5);
}
