#include "AdminMenu.h"
#include "../services/AdminService.h"
#include "../utils/ConsoleUtils.h"
#include "../constants/Strings.h"
#include <iostream>

AdminMenu::AdminMenu(const User& user) : currentUser(user) {}

void AdminMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << Strings::ADMIN_MENU_TITLE;
        std::cout << Strings::ADMIN_MENU_VIEW_SERVERS;
        std::cout << Strings::ADMIN_MENU_VIEW_DETAILS;
        std::cout << Strings::ADMIN_MENU_UPDATE_KEY;
        std::cout << Strings::ADMIN_MENU_UPDATE_STATUS;
        std::cout << Strings::ADMIN_MENU_ADD_CATEGORY;
        std::cout << Strings::ADMIN_MENU_LOGOUT;
        std::cout << Strings::ADMIN_MENU_ENTER_CHOICE;

        int choice = ConsoleUtils::getValidatedInput(1, 6);
        switch (choice) {
            case 1: listExternalServers(); break;
            case 2: viewServerDetails(); break;
            case 3: updateServerKey(); break;
            case 4: updateServerStatus(); break;
            case 5: addCategory(); break;
            case 6: back = true; break;
        }
        ConsoleUtils::pause();
    }
}

void AdminMenu::listExternalServers() {
    auto servers = AdminService::getAllServers();
    for (const auto& server : servers) {
        std::cout << Strings::ADMIN_MENU_ID << server.id
                  << " | " << Strings::ADMIN_MENU_NAME << server.name
                  << " | " << Strings::ADMIN_MENU_STATUS << server.status
                  << " | " << Strings::ADMIN_MENU_LAST_ACCESSED << server.lastAccessed << "\n";
    }
}

void AdminMenu::viewServerDetails() {
    int serverId;
    std::cout << Strings::ADMIN_MENU_ENTER_SERVER_ID;
    std::cin >> serverId;

    auto server = AdminService::getServerDetails(serverId);
    std::cout << Strings::ADMIN_MENU_ID << server.id << "\n"
              << Strings::ADMIN_MENU_NAME << server.name << "\n"
              << Strings::ADMIN_MENU_API_KEY << server.apiKey << "\n"
              << Strings::ADMIN_MENU_STATUS << server.status << "\n"
              << Strings::ADMIN_MENU_LAST_ACCESSED << server.lastAccessed << "\n";
}

void AdminMenu::updateServerKey() {
    int serverId;
    std::string newKey;
    std::cout << Strings::ADMIN_MENU_ENTER_SERVER_ID;
    std::cin >> serverId;
    std::cout << Strings::ADMIN_MENU_ENTER_NEW_KEY;
    std::cin >> newKey;

    AdminService::updateServerApiKey(serverId, newKey);
    std::cout << Strings::ADMIN_MENU_API_KEY_UPDATED;
}

void AdminMenu::updateServerStatus() {
    int serverId;
    std::string newStatus;
    std::cout << Strings::ADMIN_MENU_ENTER_SERVER_ID;
    std::cin >> serverId;
    std::cout << Strings::ADMIN_MENU_ENTER_NEW_STATUS;
    std::cin >> newStatus;

    AdminService::updateServerStatus(serverId, newStatus);
    std::cout << Strings::ADMIN_MENU_STATUS_UPDATED;
}

void AdminMenu::addCategory() {
    std::string category;
    std::cout << Strings::ADMIN_MENU_ENTER_NEW_CATEGORY;
    std::cin >> category;

    AdminService::addCategory(category);
    std::cout << Strings::ADMIN_MENU_CATEGORY_ADDED;
}
