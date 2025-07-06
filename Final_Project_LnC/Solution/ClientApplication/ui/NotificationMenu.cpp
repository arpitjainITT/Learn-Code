#include "NotificationMenu.h"
#include "../services/NotificationService.h"
#include "../utils/ConsoleUtils.h"
#include "../constants/Strings.h"
#include <iostream>

NotificationMenu::NotificationMenu(const User& user) : currentUser(user) {}

void NotificationMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << Strings::NOTIF_MENU_TITLE;
        std::cout << Strings::NOTIF_MENU_VIEW_NOTIFICATIONS;
        std::cout << Strings::NOTIF_MENU_VIEW_PREFERENCES;
        std::cout << Strings::NOTIF_MENU_CONFIGURE_CATEGORY;
        std::cout << Strings::NOTIF_MENU_CONFIGURE_KEYWORD;
        std::cout << Strings::NOTIF_MENU_BACK;
        std::cout << Strings::NOTIF_MENU_ENTER_CHOICE;

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
        std::cout << Strings::NOTIF_NO_NEW;
    } else {
        std::cout << Strings::NOTIF_DELIVERED_TITLE;
        for (const auto& n : notifications) {
            std::cout << "[" << n.id << "] " << "\n"
                      << Strings::NOTIF_TITLE << n.title << "\n"
                      << Strings::NOTIF_MESSAGE << n.message << "\n"
                      << Strings::NOTIF_STATUS << (n.read ? Strings::NOTIF_READ : Strings::NOTIF_UNREAD) << "\n\n";

            if (!n.read) {
                NotificationService::markNotificationAsRead(n.id);
                std::cout << Strings::NOTIF_MARKED_AS_READ;
            }
        }
    }

    ConsoleUtils::pause();
}

void NotificationMenu::viewPreferences() {
    auto prefs = NotificationService::getPreferences(currentUser.getId());

    std::cout << Strings::NOTIF_CATEGORY_PREFS;
    for (const auto& [cat, status] : prefs.categories)
        std::cout << "- " << cat << ": " << (status ? Strings::NOTIF_ENABLED : Strings::NOTIF_DISABLED) << "\n";

    std::cout << Strings::NOTIF_KEYWORD_PREFS;
    for (const auto& [kw, status] : prefs.keywords)
        std::cout << "- " << kw << ": " << (status ? Strings::NOTIF_ENABLED : Strings::NOTIF_DISABLED) << "\n";

    ConsoleUtils::pause();
}

void NotificationMenu::configureCategory() {
    std::string category;
    std::string enableStr;
    std::cout << Strings::NOTIF_ENTER_CATEGORY;
    std::cin >> category;
    std::cout << Strings::NOTIF_ENABLE_QUESTION;
    std::cin >> enableStr;
    bool enabled = (enableStr == "yes");

    NotificationService::setCategoryPreference(currentUser.getId(), category, enabled);
    std::cout << Strings::NOTIF_CATEGORY_UPDATED;
    ConsoleUtils::pause();
}

void NotificationMenu::configureKeyword() {
    std::string keyword;
    std::string enableStr;
    std::cout << Strings::NOTIF_ENTER_KEYWORD;
    std::cin >> keyword;
    std::cout << Strings::NOTIF_ENABLE_QUESTION;
    std::cin >> enableStr;
    bool enabled = (enableStr == "yes");

    NotificationService::setKeywordPreference(currentUser.getId(), keyword, enabled);
    std::cout << Strings::NOTIF_KEYWORD_UPDATED;
    ConsoleUtils::pause();
}
