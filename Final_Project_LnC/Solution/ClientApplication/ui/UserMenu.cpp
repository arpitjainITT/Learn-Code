#include "UserMenu.h"
#include "HeadlinesMenu.h"
#include "SavedArticlesMenu.h"
#include "SearchMenu.h"
#include "NotificationMenu.h"
#include "../utils/ConsoleUtils.h"
#include "../constants/Strings.h"
#include <iostream>

UserMenu::UserMenu(const User& user) : currentUser(user) {}

void UserMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << Strings::USER_MENU_WELCOME << currentUser.getEmail() << "\n";
        std::cout << Strings::USER_MENU_SEPARATOR;
        std::cout << Strings::USER_MENU_HEADLINES;
        std::cout << Strings::USER_MENU_SAVED_ARTICLES;
        std::cout << Strings::USER_MENU_SEARCH;
        std::cout << Strings::USER_MENU_NOTIFICATIONS;
        std::cout << Strings::USER_MENU_LOGOUT;
        std::cout << Strings::USER_MENU_ENTER_CHOICE;

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
                std::cout << Strings::INVALID_CHOICE;
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
