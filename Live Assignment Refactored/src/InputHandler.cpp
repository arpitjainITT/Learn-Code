#include "InputHandler.hpp"
#include <iostream>

std::string InputHandler::getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

double InputHandler::getDoubleInput(const std::string& prompt) {
    double input;
    std::cout << prompt;
    std::cin >> input;
    std::cin.ignore();
    return input;
}
