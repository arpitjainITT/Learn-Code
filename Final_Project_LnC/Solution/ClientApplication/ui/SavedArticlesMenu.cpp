#include "SavedArticlesMenu.h"
#include "../services/SavedArticleService.h"
#include "../utils/ConsoleUtils.h"
#include "../constants/Strings.h"
#include <iostream>
#include <vector>

SavedArticlesMenu::SavedArticlesMenu(const User& user) : currentUser(user) {}

void SavedArticlesMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << Strings::SAVED_ARTICLES_TITLE;
        std::cout << Strings::VIEW_SAVED_ARTICLES;
        std::cout << Strings::DELETE_ARTICLE;
        std::cout << Strings::BACK;
        std::cout << Strings::ENTER_CHOICE;

        int choice = ConsoleUtils::getValidatedInput(1, 3);
        switch (choice) {
            case 1:
                listSavedArticles();
                break;
            case 2: {
                std::cout << Strings::ENTER_ARTICLE_ID_TO_DELETE;
                int id;
                std::cin >> id;
                deleteArticle(id);
                break;
            }
            case 3:
                back = true;
                break;
            default:
                std::cout << Strings::INVALID_CHOICE;
        }
        ConsoleUtils::pause();
    }
}

void SavedArticlesMenu::listSavedArticles() {
    auto articles = SavedArticleService::getSavedArticles(currentUser.getId());
    if (articles.empty()) {
        std::cout << Strings::NO_SAVED_ARTICLES;
        return;
    }

    for (const auto& a : articles) {
        std::cout << "\n" << Strings::ARTICLE_ID << a.id << "\n"
                  << Strings::ARTICLE_TITLE << a.title << "\n"
                  << Strings::ARTICLE_CATEGORY << a.category << "\n"
                  << Strings::ARTICLE_SOURCE << a.source << "\n"
                  << Strings::ARTICLE_URL << a.url << "\n"
                  << Strings::ARTICLE_PUBLISHED_AT << a.createdAt << "\n";
    }
}

void SavedArticlesMenu::deleteArticle(int articleId) {
    SavedArticleService::deleteSavedArticle(currentUser.getId(), articleId);
    std::cout << Strings::ARTICLE_DELETED;
}
