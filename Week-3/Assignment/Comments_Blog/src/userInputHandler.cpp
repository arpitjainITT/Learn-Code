#include "UserInputHandler.hpp"
#include <iostream>
#include <stdexcept>

void UserInputHandler::getUserInput(std::string& blogName, int& start, int& end) {
    std::string rangeInput;

    getRawInput(blogName, rangeInput);
    parseRawInput(rangeInput, start, end);
}

void UserInputHandler::getRawInput(std::string& blogName, std::string& rangeInput) {
    std::cout << cEnterBlogName;
    std::getline(std::cin, blogName);

    std::cout << cEnterPostRange;
    std::getline(std::cin, rangeInput);
}

void UserInputHandler::parseRawInput(const std::string& rangeInput, int& start, int& end) {
    size_t dashPos = rangeInput.find('-');
    if (dashPos == std::string::npos) {
        throw std::invalid_argument(cInvalidRangeFormat);
    }

    start = std::stoi(rangeInput.substr(0, dashPos));
    end = std::stoi(rangeInput.substr(dashPos + 1));

    if (start < 1 || end < start) {
        throw std::invalid_argument(cInvalidRangeValue);
    }
}
