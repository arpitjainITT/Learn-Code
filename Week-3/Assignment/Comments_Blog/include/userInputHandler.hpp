#ifndef USER_INPUT_HANDLER_HPP
#define USER_INPUT_HANDLER_HPP

#include "constants.hpp"

class UserInputHandler {
public:
    static void getUserInput(std::string& blogName, int& start, int& end);

private:
    static void getRawInput(std::string& blogName, std::string& rangeInput);
    static void parseRawInput(const std::string& rangeInput, int& start, int& end);
};

#endif 
