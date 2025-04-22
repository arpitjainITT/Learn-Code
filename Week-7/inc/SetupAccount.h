#ifndef SETUPACCOUNT_H
#define SETUPACCOUNT_H

#include "Account.h"
#include "Card.h"
#include <vector>

class SetupAccount {
private:
    long long accountCounter = 0;
    long long cardCounter = 0; 
    std::vector<Card> cards;
public:
    std::string generateCardNumber() ;
    std::string generateAccountNumber() ;
    Card createAccount();
};

#endif