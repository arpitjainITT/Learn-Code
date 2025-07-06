#include "HeadlinesMenu.h"
#include "../services/NewsService.h"
#include "../utils/ConsoleUtils.h"
#include "../constants/Strings.h"
#include <iostream>
#include <vector>

HeadlinesMenu::HeadlinesMenu(const User& user) : currentUser(user) {}

void HeadlinesMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << Strings::HEADLINES_MENU_TITLE;
        std::cout << Strings::HEADLINES_MENU_TODAY;
        std::cout << Strings::HEADLINES_MENU_DATE_RANGE;
        std::cout << Strings::HEADLINES_MENU_LOGOUT;
        std::cout << Strings::HEADLINES_MENU_ENTER_CHOICE;

        int choice = ConsoleUtils::getValidatedInput(1, 3);
        switch (choice) {
            case 1:
                showTodayMenu();
                break;
            case 2:
                showDateRangeMenu();
                break;
            case 3:
                back = true;
                break;
            default:
                std::cout << Strings::HEADLINES_MENU_INVALID_CHOICE;
        }
    }
}

void HeadlinesMenu::showTodayMenu() {
    // ConsoleUtils::clear();
    std::cout << Strings::HEADLINES_TODAY_TITLE;
    std::cout << Strings::HEADLINES_TODAY_ALL;
    std::cout << Strings::HEADLINES_TODAY_BUSINESS;
    std::cout << Strings::HEADLINES_TODAY_ENTERTAINMENT;
    std::cout << Strings::HEADLINES_TODAY_SPORTS;
    std::cout << Strings::HEADLINES_TODAY_TECHNOLOGY;
    std::cout << Strings::HEADLINES_TODAY_BACK;

    int choice = ConsoleUtils::getValidatedInput(1, 6);
    std::string category;
    switch (choice) {
        case 1: category = "all"; break;
        case 2: category = "business"; break;
        case 3: category = "entertainment"; break;
        case 4: category = "sports"; break;
        case 5: category = "technology"; break;
        case 6: return;
    }

    fetchAndDisplayArticles(category);
}

void HeadlinesMenu::showDateRangeMenu() {
    std::string startDate, endDate;
    std::cout << Strings::HEADLINES_ENTER_START_DATE;
    std::cin >> startDate;
    std::cout << Strings::HEADLINES_ENTER_END_DATE;
    std::cin >> endDate;

    fetchAndDisplayArticles("all", startDate, endDate);
}

void HeadlinesMenu::fetchAndDisplayArticles(const std::string& category, const std::string& startDate, const std::string& endDate) {
    auto articles = NewsService::getArticles(category, startDate, endDate);
    if (articles.empty()) {
        std::cout << Strings::HEADLINES_NO_ARTICLES;
        ConsoleUtils::pause();
        return;
    }

    for (size_t i = 0; i < articles.size(); ++i) {
        std::cout << "\n[" << i + 1 << "] " << articles[i].title << "\n"
                  << Strings::HEADLINES_SOURCE << articles[i].source << "\n"
                  << Strings::HEADLINES_CATEGORY << articles[i].category << "\n";
    }

    std::cout << Strings::HEADLINES_SELECT_ARTICLE;
    int choice = ConsoleUtils::getValidatedInput(0, articles.size());
    if (choice == 0) return;

    showArticleOptions(articles[choice - 1].id);
}

void HeadlinesMenu::showArticleOptions(int articleId) {
    // ConsoleUtils::clear();
    std::cout << Strings::HEADLINES_ARTICLE_OPTIONS_SAVE;
    std::cout << Strings::HEADLINES_ARTICLE_OPTIONS_LIKE;
    std::cout << Strings::HEADLINES_ARTICLE_OPTIONS_DISLIKE;
    std::cout << Strings::HEADLINES_ARTICLE_OPTIONS_BACK;
    std::cout << Strings::HEADLINES_ARTICLE_OPTIONS_ENTER_CHOICE;

    int choice = ConsoleUtils::getValidatedInput(1, 4);
    switch (choice) {
        case 1:
            NewsService::saveArticle(currentUser.getId(), articleId);
            std::cout << Strings::HEADLINES_ARTICLE_SAVED;
            break;
        case 2:
            NewsService::likeArticle(currentUser.getId(), articleId);
            std::cout << Strings::HEADLINES_ARTICLE_LIKED;
            break;
        case 3:
            NewsService::dislikeArticle(currentUser.getId(), articleId);
            std::cout << Strings::HEADLINES_ARTICLE_DISLIKED;
            break;
        case 4:
            return;
    }
    ConsoleUtils::pause();
}
