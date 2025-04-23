#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "IWallet.h"

class Customer {
private:
    std::string firstName;
    std::string lastName;
    IWallet* myWallet;  

public:
    Customer(const std::string& first, const std::string& last, IWallet* wallet);

    bool pay(float amount);
    void displayInfo() const;
};

#endif
