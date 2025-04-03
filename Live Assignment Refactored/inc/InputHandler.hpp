#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "constants.hpp"

class InputHandler {
public:
    static std::string getStringInput(const std::string& prompt);
    static double getDoubleInput(const std::string& prompt);
};

#endif 
