#include "AdminMenu.h"
#include "../services/AdminService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>

AdminMenu::AdminMenu(const User& user) : currentUser(user) {}

void AdminMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << "========== ADMIN PANEL ==========\n";
        std::cout << "1. View External Servers\n";
        std::cout << "2. View Server Details\n";
        std::cout << "3. Update Server API Key\n";
        std::cout << "4. Update Server Status\n";
        std::cout << "5. Add New Category\n";
        std::cout << "6. Logout\n";
        std::cout << "Enter your choice: ";

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
        std::cout << "ID: " << server.id
                  << " | Name: " << server.name
                  << " | Status: " << server.status
                  << " | Last Accessed: " << server.lastAccessed << "\n";
    }
}

void AdminMenu::viewServerDetails() {
    int serverId;
    std::cout << "Enter server ID: ";
    std::cin >> serverId;

    auto server = AdminService::getServerDetails(serverId);
    std::cout << "ID: " << server.id << "\n"
              << "Name: " << server.name << "\n"
              << "API Key: " << server.apiKey << "\n"
              << "Status: " << server.status << "\n"
              << "Last Accessed: " << server.lastAccessed << "\n";
}

void AdminMenu::updateServerKey() {
    int serverId;
    std::string newKey;
    std::cout << "Enter server ID: ";
    std::cin >> serverId;
    std::cout << "Enter new API key: ";
    std::cin >> newKey;

    AdminService::updateServerApiKey(serverId, newKey);
    std::cout << "API key updated.\n";
}

void AdminMenu::updateServerStatus() {
    int serverId;
    std::string newStatus;
    std::cout << "Enter server ID: ";
    std::cin >> serverId;
    std::cout << "Enter new status (active/inactive): ";
    std::cin >> newStatus;

    AdminService::updateServerStatus(serverId, newStatus);
    std::cout << "Status updated.\n";
}

void AdminMenu::addCategory() {
    std::string category;
    std::cout << "Enter new category name: ";
    std::cin >> category;

    AdminService::addCategory(category);
    std::cout << "Category added.\n";
}
