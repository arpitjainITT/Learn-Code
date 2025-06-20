#include "UserMenu.h"
#include "HeadlinesMenu.h"
#include "SavedArticlesMenu.h"
#include "SearchMenu.h"
#include "NotificationMenu.h"
#include "../utils/ConsoleUtils.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>

UserMenu::UserMenu(const User& u) : user(u) {}

void UserMenu::show() {
    int choice;
    // to fetch the real time of the system
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* local_tm = std::localtime(&now_c);
    do {
        // ConsoleUtils::clear();
        std::cout << "Welcome to the News Application, " << user.getUsername() << "!\n";
        std::cout << "Date: " << std::put_time(local_tm, "%d-%b-%Y") << " | Time: " << std::put_time(local_tm, "%I:%M %p") << "\n";
        std::cout << "===========================\n";
        std::cout << "1. Headlines\n";
        std::cout << "2. Saved Articles\n";
        std::cout << "3. Search\n";
        std::cout << "4. Notifications\n";
        std::cout << "5. Logout\n";
        std::cout << "===========================\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                HeadlinesMenu(user).show();
                break;
            case 2:
                SavedArticlesMenu(user).show();
                break;
            case 3:
                SearchMenu(user).show();
                break;
            case 4:
                NotificationMenu(user).show();
                break;
            case 5:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}
