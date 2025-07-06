#include "ConsoleUtils.h"
#include <cstdlib>

void ConsoleUtils::clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int ConsoleUtils::getValidatedInput(int min, int max) {
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again: ";
        } else {
            return choice;
        }
    }
}

void ConsoleUtils::pause() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();  
}