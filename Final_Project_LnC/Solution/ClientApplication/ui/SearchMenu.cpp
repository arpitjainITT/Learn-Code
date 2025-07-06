#include "SearchMenu.h"
#include "../services/NewsService.h"
#include "../utils/ConsoleUtils.h"
#include "../constants/Strings.h"
#include <iostream>
#include <algorithm>

SearchMenu::SearchMenu(const User& user) : currentUser(user) {}

void SearchMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << Strings::SEARCH_MENU_TITLE;
        std::cout << Strings::SEARCH_MENU_SEARCH_ARTICLES;
        std::cout << Strings::SEARCH_MENU_BACK;
        std::cout << Strings::SEARCH_MENU_ENTER_CHOICE;
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
    
    std::cout << Strings::SEARCH_MENU_ENTER_KEYWORD;
    std::cin >> keyword;
    std::cout << Strings::SEARCH_MENU_ENTER_START_DATE;
    std::cin >> startDate;
    std::cout << Strings::SEARCH_MENU_ENTER_END_DATE;
    std::cin >> endDate;
    std::cout << Strings::SEARCH_MENU_SORT_BY;
    std::cin >> sortBy;

    if (sortBy != "likes" && sortBy != "dislikes") {
        sortBy = "";
    }

    auto results = NewsService::searchArticles(keyword, startDate, endDate, sortBy);

    if (results.empty()) {
        std::cout << Strings::SEARCH_MENU_NO_MATCH;
        ConsoleUtils::pause();
        return;
    }

    std::cout << Strings::SEARCH_MENU_RESULTS_TITLE;
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "[" << (i + 1) << "] " << results[i].title << "\n"
                  << Strings::SEARCH_MENU_CATEGORY << results[i].category << "\n"
                  << Strings::SEARCH_MENU_SOURCE << results[i].source << "\n"
                  << Strings::SEARCH_MENU_PUBLISHED_AT << results[i].createdAt << "\n";
    }

    std::cout << Strings::SEARCH_MENU_ENTER_ARTICLE_NUMBER;
    int choice = ConsoleUtils::getValidatedInput(0, results.size());
    if (choice > 0) {
        NewsService::saveArticle(currentUser.getId(), results[choice - 1].id);
        std::cout << Strings::SEARCH_MENU_ARTICLE_SAVED;
    }

    ConsoleUtils::pause();
}