#include "Card.h"
#include "Exceptions.h"

Card::Card(std::string number, Account* accountNo) : cardNumber(number), account(accountNo) {}

bool Card::validatePIN(int inputPIN) {
    if (account->isBlocked()) {
        throw CardBlockedException();
    }
    if (!account->verifyPIN(inputPIN)) {
        if (account->isBlocked()) throw CardBlockedException();
        return false;
    }
    return true;
}

Account* Card::getAccount() { 
    return account; 
}