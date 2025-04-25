#include "Customer.hpp"
#include "InputHandler.hpp"
#include <iostream>

void Customer::getUserInput() {
    customerId = InputHandler::getStringInput(cEnter + cCustomer + cID); 
    customerName = InputHandler::getStringInput(cEnter + cCustomer + cName);
    customerEmail = InputHandler::getStringInput(cEnter + cCustomer + cEmail);
    customerAddress = InputHandler::getStringInput(cEnter + cCustomer + cAddress);
    customerContacts = InputHandler::getStringInput(cEnter + cCustomer + cPhone);
    customerMembership = InputHandler::getStringInput(cEnter + cMembershipLevel);
}
