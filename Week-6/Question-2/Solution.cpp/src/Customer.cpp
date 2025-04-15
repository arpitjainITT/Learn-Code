#include "Customer.h"
#include <iostream>

Customer::Customer(const std::string& first, const std::string& last, IWallet* wallet)
    : firstName(first), lastName(last), wallet(wallet) {}

bool Customer::pay(float amount) {
    return wallet->withdraw(amount);
}

void Customer::displayInfo() const {
    std::cout << "Customer: " << firstName << " " << lastName
              << ", Balance: $" << wallet->getBalance() << std::endl;
}
