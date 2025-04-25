#include "OutputHandler.hpp"
#include <iostream>

void OutputHandler::sendCustomerNotification(const std::string& email, const std::string& message) {
    std::cout << cSendingEmailTo << email << ": " << message << std::endl;
}

void OutputHandler::sendAdminNotification(const std::string& message) {
    std::cout << cAdmin + cNotification << message << std::endl;
}

void OutputHandler::displayMessage(const std::string& message) {
    std::cout << message << std::endl;
}
