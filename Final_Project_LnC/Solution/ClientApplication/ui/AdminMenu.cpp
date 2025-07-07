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
        std::cout << Strings::ADMIN_MENU_REVIEW_REPORTS;
        std::cout << Strings::ADMIN_MENU_HIDE_ARTICLE;
        std::cout << Strings::ADMIN_MENU_HIDE_CATEGORY;
        std::cout << Strings::ADMIN_MENU_MANAGE_KEYWORDS;
        std::cout << Strings::ADMIN_MENU_SHOW_HIDDEN_CATEGORIES;
        std::cout << Strings::ADMIN_MENU_SHOW_FILTERED_KEYWORDS;
        std::cout << Strings::ADMIN_MENU_LOGOUT;
        std::cout << Strings::ADMIN_MENU_ENTER_CHOICE;

        int choice = ConsoleUtils::getValidatedInput(1, 12);
        switch (choice) {
            case 1: listExternalServers(); break;
            case 2: viewServerDetails(); break;
            case 3: updateServerKey(); break;
            case 4: updateServerStatus(); break;
            case 5: addCategory(); break;
            case 6: reviewReportedArticles(); break;
            case 7: hideUnhideArticle(); break;
            case 8: hideUnhideCategory(); break;
            case 9: manageFilteredKeywords(); break;
            case 10: showHiddenCategories(); break;
            case 11: showFilteredKeywords(); break;
            case 12: back = true; break;
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

void AdminMenu::reviewReportedArticles() {
    auto articles = AdminService::getReportedArticles();
    if (articles.empty()) {
        std::cout << "No reported articles.\n";
        return;
    }
    for (const auto& a : articles) {
        std::cout << "ID: " << a.id << " | Title: " << a.title << " | Reports: " << a.reportCount << " | Hidden: " << (a.isHidden ? "Yes" : "No") << "\n";
    }
    std::cout << "Enter article ID to hide/unhide or 0 to return: ";
    int id; std::cin >> id;
    if (id == 0) return;
    std::cout << "[1] Hide  [2] Unhide\nEnter choice: ";
    int ch; std::cin >> ch;
    if (ch == 1) AdminService::hideArticle(id);
    else if (ch == 2) AdminService::unhideArticle(id);
}

void AdminMenu::hideUnhideArticle() {
    std::cout << "Enter article ID: ";
    int id; std::cin >> id;
    std::cout << "[1] Hide  [2] Unhide\nEnter choice: ";
    int ch; std::cin >> ch;
    if (ch == 1) AdminService::hideArticle(id);
    else if (ch == 2) AdminService::unhideArticle(id);
}

void AdminMenu::hideUnhideCategory() {
    std::cout << "Enter category: ";
    std::string cat; std::cin >> cat;
    std::cout << "[1] Hide  [2] Unhide\nEnter choice: ";
    int ch; std::cin >> ch;
    if (ch == 1) AdminService::hideCategory(cat);
    else if (ch == 2) AdminService::unhideCategory(cat);
}

void AdminMenu::manageFilteredKeywords() {
    std::cout << "[1] Add Filtered Keyword  [2] Remove Filtered Keyword\nEnter choice: ";
    int ch; std::cin >> ch;
    std::cout << "Enter keyword: ";
    std::string kw; std::cin >> kw;
    if (ch == 1) AdminService::addFilteredKeyword(kw);
    else if (ch == 2) AdminService::removeFilteredKeyword(kw);
}

void AdminMenu::showHiddenCategories() {
    auto cats = AdminService::getHiddenCategories();
    std::cout << "Hidden Categories:\n";
    for (const auto& c : cats) std::cout << c << "\n";
}

void AdminMenu::showFilteredKeywords() {
    auto kws = AdminService::getFilteredKeywords();
    std::cout << "Filtered Keywords:\n";
    for (const auto& k : kws) std::cout << k << "\n";
}
