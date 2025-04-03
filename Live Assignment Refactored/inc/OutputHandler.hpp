#ifndef OUTPUT_HANDLER_HPP
#define OUTPUT_HANDLER_HPP

#include "constants.hpp"

class OutputHandler {
public:
    static void sendCustomerNotification(const std::string& email, const std::string& message);
    static void sendAdminNotification(const std::string& message);
    static void displayMessage(const std::string& message);
};

#endif 
