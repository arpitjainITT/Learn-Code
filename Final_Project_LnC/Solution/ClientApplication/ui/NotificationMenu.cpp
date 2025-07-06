#include "NotificationMenu.h"
#include "../services/NotificationService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>

NotificationMenu::NotificationMenu(const User& user) : currentUser(user) {}

void NotificationMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << "========== NOTIFICATION MENU ==========\n";
        std::cout << "1. View Notifications\n";
        std::cout << "2. View Preferences\n";
        std::cout << "3. Configure Category Notifications\n";
        std::cout << "4. Configure Keyword Notifications\n";
        std::cout << "5. Back\n";
        std::cout << "Enter choice: ";

        int choice = ConsoleUtils::getValidatedInput(1, 5);
        switch (choice) {
            case 1: viewDeliveredNotifications(); break;
            case 2: viewPreferences(); break;
            case 3: configureCategory(); break;
            case 4: configureKeyword(); break;
            case 5: back = true; break;
        }
    }
}

void NotificationMenu::viewDeliveredNotifications() {
    auto notifications = NotificationService::getNotifications(currentUser.getId());

    if (notifications.empty()) {
        std::cout << "No new notifications.\n";
    } else {
        std::cout << "=== Delivered Notifications ===\n";
        for (const auto& n : notifications) {
            std::cout << "[" << n.id << "] " << "\n"
                      << "Title: " << n.title << "\n"
                      << "Message: " << n.message << "\n"
                      << "Status: " << (n.read ? "Read" : "Unread") << "\n\n";

            if (!n.read) {
                NotificationService::markNotificationAsRead(n.id);
                std::cout << "(Marked as read)\n";
            }
        }
    }

    ConsoleUtils::pause();
}

void NotificationMenu::viewPreferences() {
    auto prefs = NotificationService::getPreferences(currentUser.getId());

    std::cout << "\n--- Category Preferences ---\n";
    for (const auto& [cat, status] : prefs.categories)
        std::cout << "- " << cat << ": " << (status ? "Enabled" : "Disabled") << "\n";

    std::cout << "\n--- Keyword Preferences ---\n";
    for (const auto& [kw, status] : prefs.keywords)
        std::cout << "- " << kw << ": " << (status ? "Enabled" : "Disabled") << "\n";

    ConsoleUtils::pause();
}

void NotificationMenu::configureCategory() {
    std::string category;
    std::string enableStr;
    std::cout << "Enter category name: ";
    std::cin >> category;
    std::cout << "Enable? (yes/no): ";
    std::cin >> enableStr;
    bool enabled = (enableStr == "yes");

    NotificationService::setCategoryPreference(currentUser.getId(), category, enabled);
    std::cout << "Category preference updated.\n";
    ConsoleUtils::pause();
}

void NotificationMenu::configureKeyword() {
    std::string keyword;
    std::string enableStr;
    std::cout << "Enter keyword: ";
    std::cin >> keyword;
    std::cout << "Enable? (yes/no): ";
    std::cin >> enableStr;
    bool enabled = (enableStr == "yes");

    NotificationService::setKeywordPreference(currentUser.getId(), keyword, enabled);
    std::cout << "Keyword preference updated.\n";
    ConsoleUtils::pause();
}
