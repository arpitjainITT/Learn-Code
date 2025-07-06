#include "SearchMenu.h"
#include "../services/NewsService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>
#include <algorithm>

SearchMenu::SearchMenu(const User& user) : currentUser(user) {}

void SearchMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << "========== SEARCH ==========\n";
        std::cout << "1. Search Articles\n";
        std::cout << "2. Back\n";
        std::cout << "Enter choice: ";
        int choice = ConsoleUtils::getValidatedInput(1, 2);
        switch (choice) {
            case 1:
                performSearch();
                break;
            case 2:
                back = true;
                break;
        }
    }
}

void SearchMenu::performSearch() {
    std::string keyword, startDate, endDate, sortBy;
    
    std::cout << "Enter keyword to search: ";
    std::cin >> keyword;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> startDate;
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::cin >> endDate;
    std::cout << "Sort by (likes/dislikes/none): ";
    std::cin >> sortBy;

    if (sortBy != "likes" && sortBy != "dislikes") {
        sortBy = "";
    }

    auto results = NewsService::searchArticles(keyword, startDate, endDate, sortBy);

    if (results.empty()) {
        std::cout << "No matching articles found.\n";
        ConsoleUtils::pause();
        return;
    }

    std::cout << "\n=== Search Results ===\n";
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "[" << (i + 1) << "] " << results[i].title << "\n"
                  << "   Category: " << results[i].category << "\n"
                  << "   Source: " << results[i].source << "\n"
                  << "   Published At: " << results[i].createdAt << "\n";
    }

    std::cout << "\nEnter article number to save (0 to skip): ";
    int choice = ConsoleUtils::getValidatedInput(0, results.size());
    if (choice > 0) {
        NewsService::saveArticle(currentUser.getId(), results[choice - 1].id);
        std::cout << "Article saved.\n";
    }

    ConsoleUtils::pause();
}