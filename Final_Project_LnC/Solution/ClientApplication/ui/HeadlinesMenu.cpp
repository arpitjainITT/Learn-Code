#include "HeadlinesMenu.h"
#include "../services/NewsService.h"
#include "../services/SavedArticleService.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>

HeadlinesMenu::HeadlinesMenu(const User& user) : user(user) {}
void HeadlinesMenu::show() {
    int mainChoice;
    do {
        std::cout << "=== Headlines Menu ===\n";
        std::cout << "1. Today\n2. Date Range\n3. Back\n";
        std::cout << "Enter your choice: ";
        std::cin >> mainChoice;

        switch (mainChoice) {
            case 1: {
                int categoryChoice;
                do {
                    std::cout << "=== Today's Headlines ===\n";
                    std::cout << "1. All\n2. Business\n3. Entertainment\n4. Sports\n5. Technology\n6. Politics\n7. Back\n";
                    std::cout << "Select category: ";
                    std::cin >> categoryChoice;

                    std::string category;
                    switch (categoryChoice) {
                        case 1: category = "all"; break;
                        case 2: category = "Business"; break;
                        case 3: category = "Entertainment"; break;
                        case 4: category = "Sports"; break;
                        case 5: category = "Technology"; break;
                        case 6: category = "Politics"; break;
                        case 7: break;
                        default: std::cout << "Invalid category.\n"; continue;
                    }

                    if (categoryChoice != 7) {
                        auto headlines = NewsService::getTodayHeadlines(category);
                        NewsService::printArticlesTable(headlines);
                    
                        int userAction;
                        std::cout << "\n1. Save Article\n2. Back\n3. Logout\nChoice: ";
                        std::cin >> userAction;
                    
                        if (userAction == 1) {
                            std::string articleId;
                            std::cout << "Enter Article ID to save: ";
                            std::cin >> articleId;
                            SavedArticleService::saveArticle(user, articleId);
                        } else if (userAction == 3) {
                            exit(0);
                        }
                    }

                } while (categoryChoice != 7);
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
