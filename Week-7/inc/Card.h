#ifndef CARD_H
#define CARD_H

#include "Account.h"

class Card {
private:
    std::string cardNumber;
    Account* account;
public:
    Card(std::string number, Account* accountNo);
    bool validatePIN(int inputPIN);
    Account* getAccount();
};

#endif 