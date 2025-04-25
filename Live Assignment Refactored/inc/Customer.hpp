#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "constants.hpp"

class Customer {
public:
    std::string customerId;
    std::string customerName;
    std::string customerEmail;
    std::string customerAddress;
    std::string customerContacts;
    std::string customerMembership;

    Customer() = default;
    void getUserInput();  
};

#endif 
