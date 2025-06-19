#include "HeadlinesMenu.h"
#include "../services/NewsService.h"
#include "../services/SavedArticleService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>

HeadlinesMenu::HeadlinesMenu(const User& u) : user(u) {}

void HeadlinesMenu::show() {
    int mainChoice;
    do {
        ConsoleUtils::clear();
        std::cout << "=== Headlines Menu ===\n";
        std::cout << "1. Today\n";
        std::cout << "2. Date Range\n";
        std::cout << "3. Back\n";
        std::cout << "Enter your choice: ";
        std::cin >> mainChoice;

        switch (mainChoice) {
            case 1: {
                int catChoice;
                do {
                    ConsoleUtils::clear();
                    std::cout << "=== Today's Headlines ===\n";
                    std::cout << "1. All\n2. Business\n3. Entertainment\n4. Sports\n5. Technology\n6. Back\n";
                    std::cout << "Select category: ";
                    std::cin >> catChoice;

                    std::string category;
                    switch (catChoice) {
                        case 1: category = "all"; break;
                        case 2: category = "business"; break;
                        case 3: category = "entertainment"; break;
                        case 4: category = "sports"; break;
                        case 5: category = "technology"; break;
                        case 6: continue;
                        default: std::cout << "Invalid category.\n"; continue;
                    }

                    auto articles = NewsService::getTodayHeadlines(category);
                    NewsService::printArticlesTable(articles);

                    int action;
                    std::cout << "\n1. Save Article\n2. Back\n3. Logout\nChoice: ";
                    std::cin >> action;

                    if (action == 1) {
                        std::string articleId;
                        std::cout << "Enter Article ID to save: ";
                        std::cin >> articleId;
                        SavedArticleService::saveArticle(user, articleId);
                    } else if (action == 3) {
                        exit(0);
                    }

                } while (catChoice != 6);
                break;
            }
            case 2:
                std::cout << "[Date Range] - Not implemented yet\n";
                break;
            case 3:
                break;
            default:
                std::cout << "Invalid choice.\n";
        }

    } while (mainChoice != 3);
}
