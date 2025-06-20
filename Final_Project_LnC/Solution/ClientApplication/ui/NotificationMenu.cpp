#include "NotificationMenu.h"
#include "../services/NotificationService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>

NotificationMenu::NotificationMenu(const User& u) : user(u) {}

void NotificationMenu::show() {
    int choice;
    do {
        // ConsoleUtils::clear();
        std::cout << "=== Notifications ===\n";
        std::cout << "1. View Notifications\n";
        std::cout << "2. Configure Notifications\n";
        std::cout << "3. Back\n";
        std::cout << "4. Logout\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                NotificationService::viewNotifications(user);
                break;
            case 2:
                NotificationService::configureNotifications(user);
                break;
            case 4:
                exit(0);
        }
    } while (choice != 3);
}
