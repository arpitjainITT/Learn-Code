#include "SavedArticlesMenu.h"
#include "../services/SavedArticleService.h"
#include "../services/NewsService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>

SavedArticlesMenu::SavedArticlesMenu(const User& u) : user(u) {}

void SavedArticlesMenu::show() {
    ConsoleUtils::clear();
    std::cout << "=== Saved Articles ===\n";

    auto articles = SavedArticleService::getSavedArticles(user);
    NewsService::printArticlesTable(articles);

    int choice;
    std::cout << "\n1. Delete Article\n2. Back\n3. Logout\nChoice: ";
    std::cin >> choice;

    if (choice == 1) {
        std::string id;
        std::cout << "Enter Article ID to delete: ";
        std::cin >> id;
        SavedArticleService::deleteArticle(user, id);
    } else if (choice == 3) {
        exit(0);
    }
}
