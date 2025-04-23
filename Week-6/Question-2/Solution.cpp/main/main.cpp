#include <iostream>
#include "Customer.h"
#include "Wallet.h"

int main() {
    Wallet* myWallet = new Wallet(10.0f);
    Customer customer("Arpit", "Jain", myWallet);

    float payment = 2.0f;
    std::cout << "Attempting to pay $" << payment << "...\n";

    if (customer.pay(payment)) {
        std::cout << "Payment successful!\n";
    } else {
        std::cout << "Not enough funds. Try again later.\n";
    }

    customer.displayInfo();

    delete myWallet; 
    return 0;
}
