#include "UserMenu.h"
#include "HeadlinesMenu.h"
#include "SavedArticlesMenu.h"
#include "SearchMenu.h"
#include "NotificationMenu.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>

UserMenu::UserMenu(const User& user) : currentUser(user) {}

void UserMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << "Welcome to the News Application, " << currentUser.getEmail() << "\n";
        std::cout << "====================================\n";
        std::cout << "1. Headlines\n";
        std::cout << "2. Saved Articles\n";
        std::cout << "3. Search\n";
        std::cout << "4. Notifications\n";
        std::cout << "5. Logout\n";
        std::cout << "Enter choice: ";

        int choice = ConsoleUtils::getValidatedInput(1, 5);
        switch (choice) {
            case 1:
                showHeadlinesMenu();
                break;
            case 2:
                showSavedArticlesMenu();
                break;
            case 3:
                showSearchMenu();
                break;
            case 4:
                showNotificationMenu();
                break;
            case 5:
                back = true;
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
}

void UserMenu::showHeadlinesMenu() {
    HeadlinesMenu menu(currentUser);
    menu.display();
}

void UserMenu::showSavedArticlesMenu() {
    SavedArticlesMenu menu(currentUser);
    menu.display();
}

void UserMenu::showSearchMenu() {
    SearchMenu menu(currentUser);
    menu.display();
}

void UserMenu::showNotificationMenu() {
    NotificationMenu menu(currentUser);
    menu.display();
}
