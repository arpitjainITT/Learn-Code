#include "SavedArticlesMenu.h"
#include "../services/SavedArticleService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>
#include <vector>

SavedArticlesMenu::SavedArticlesMenu(const User& user) : currentUser(user) {}

void SavedArticlesMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << "==== SAVED ARTICLES ====\n";
        std::cout << "1. View Saved Articles\n";
        std::cout << "2. Delete Article\n";
        std::cout << "3. Back\n";
        std::cout << "Enter your choice: ";

        int choice = ConsoleUtils::getValidatedInput(1, 3);
        switch (choice) {
            case 1:
                listSavedArticles();
                break;
            case 2: {
                std::cout << "Enter Article ID to delete: ";
                int id;
                std::cin >> id;
                deleteArticle(id);
                break;
            }
            case 3:
                back = true;
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
        ConsoleUtils::pause();
    }
}

void SavedArticlesMenu::listSavedArticles() {
    auto articles = SavedArticleService::getSavedArticles(currentUser.getId());
    if (articles.empty()) {
        std::cout << "No saved articles.\n";
        return;
    }

    for (const auto& a : articles) {
        std::cout << "\nID: " << a.id << "\n"
                  << "Title: " << a.title << "\n"
                  << "Category: " << a.category << "\n"
                  << "Source: " << a.source << "\n"
                  << "URL: " << a.url << "\n"
                  << "Published At: " << a.createdAt << "\n";
    }
}

void SavedArticlesMenu::deleteArticle(int articleId) {
    SavedArticleService::deleteSavedArticle(currentUser.getId(), articleId);
    std::cout << "Article deleted.\n";
}
