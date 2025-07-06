#include "ConsoleUtils.h"
#include "../constants/Strings.h"
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
            std::cout << Strings::UTILS_INVALID_INPUT;
        } else {
            return choice;
        }
    }
}

void ConsoleUtils::pause() {
    std::cout << Strings::UTILS_PRESS_ENTER;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();  
}