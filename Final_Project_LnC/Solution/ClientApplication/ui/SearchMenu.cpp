#include "SearchMenu.h"
#include "../services/NewsService.h"
#include "../services/SavedArticleService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>

SearchMenu::SearchMenu(const User& u) : user(u) {}

void SearchMenu::show() {
    ConsoleUtils::clear();
    std::cout << "=== Search Articles ===\n";
    std::string keyword, startDate, endDate;
    std::cout << "Enter keyword: ";
    std::cin >> keyword;
    std::cout << "Start Date (YYYY-MM-DD): ";
    std::cin >> startDate;
    std::cout << "End Date (YYYY-MM-DD): ";
    std::cin >> endDate;

    auto articles = NewsService::searchArticles(keyword, startDate, endDate);
    NewsService::printArticlesTable(articles);

    int choice;
    std::cout << "\n1. Save Article\n2. Back\n3. Logout\nChoice: ";
    std::cin >> choice;
    if (choice == 1) {
        std::string id;
        std::cout << "Enter Article ID to save: ";
        std::cin >> id;
        SavedArticleService::saveArticle(user, id);
    } else if (choice == 3) {
        exit(0);
    }
}
