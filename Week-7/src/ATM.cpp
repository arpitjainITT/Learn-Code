#include "ATM.h"
#include "Exceptions.h"
#include <iostream>

ATM::ATM(std::string id, double cash, IBankServer* bankServer)
    : atmId(id), availableCash(cash), server(bankServer) {}

void ATM::withdraw(Card& card, int pin, double amount) {
    if (!server->isConnected()) {
        throw ConnectionFailedException();
    }

    if (!card.validatePIN(pin)) {
        std::cout << cInvalidPin << std::endl;
        return;
    }

    if (amount > availableCash) {
        throw ATMCashUnavailableException();
    }

    Account* account = card.getAccount();
    account->withdraw(amount);
    availableCash -= amount;

    std::cout << cWithdrawlSuccessful << account->getBalance() << std::endl;
}