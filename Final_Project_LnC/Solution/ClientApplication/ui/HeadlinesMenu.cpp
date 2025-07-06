#include "HeadlinesMenu.h"
#include "../services/NewsService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>
#include <vector>

HeadlinesMenu::HeadlinesMenu(const User& user) : currentUser(user) {}

void HeadlinesMenu::display() {
    bool back = false;
    while (!back) {
        // ConsoleUtils::clear();
        std::cout << "========== HEADLINES ==========\n";
        std::cout << "1. Today\n";
        std::cout << "2. Date range\n";
        std::cout << "3. Logout\n";
        std::cout << "Enter choice: ";

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
                std::cout << "Invalid choice.\n";
        }
    }
}

void HeadlinesMenu::showTodayMenu() {
    // ConsoleUtils::clear();
    std::cout << "---- Today's Headlines ----\n";
    std::cout << "1. All\n";
    std::cout << "2. Business\n";
    std::cout << "3. Entertainment\n";
    std::cout << "4. Sports\n";
    std::cout << "5. Technology\n";
    std::cout << "6. Back\n";

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
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> startDate;
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::cin >> endDate;

    fetchAndDisplayArticles("all", startDate, endDate);
}

void HeadlinesMenu::fetchAndDisplayArticles(const std::string& category, const std::string& startDate, const std::string& endDate) {
    auto articles = NewsService::getArticles(category, startDate, endDate);
    if (articles.empty()) {
        std::cout << "No articles found.\n";
        ConsoleUtils::pause();
        return;
    }

    for (size_t i = 0; i < articles.size(); ++i) {
        std::cout << "\n[" << i + 1 << "] " << articles[i].title << "\n"
                  << "Source: " << articles[i].source << "\n"
                  << "Category: " << articles[i].category << "\n";
    }

    std::cout << "\nSelect article to view details and options (0 to go back): ";
    int choice = ConsoleUtils::getValidatedInput(0, articles.size());
    if (choice == 0) return;

    showArticleOptions(articles[choice - 1].id);
}

void HeadlinesMenu::showArticleOptions(int articleId) {
    // ConsoleUtils::clear();
    std::cout << "1. Save Article\n";
    std::cout << "2. Like Article\n";
    std::cout << "3. Dislike Article\n";
    std::cout << "4. Back\n";
    std::cout << "Enter choice: ";

    int choice = ConsoleUtils::getValidatedInput(1, 4);
    switch (choice) {
        case 1:
            NewsService::saveArticle(currentUser.getId(), articleId);
            std::cout << "Article saved.\n";
            break;
        case 2:
            NewsService::likeArticle(currentUser.getId(), articleId);
            std::cout << "Article liked.\n";
            break;
        case 3:
            NewsService::dislikeArticle(currentUser.getId(), articleId);
            std::cout << "Article disliked.\n";
            break;
        case 4:
            return;
    }
    ConsoleUtils::pause();
}
