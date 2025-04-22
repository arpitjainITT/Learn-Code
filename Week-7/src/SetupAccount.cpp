#include "SetupAccount.h"
#include <iostream>
#include <iomanip>

std::string SetupAccount::generateCardNumber() {
    long long result = std::stoll(initialCardNumber) + ++cardCounter;
    return std::to_string(result);
}

std::string SetupAccount::generateAccountNumber() {
    long long result = std::stoll(initialAccountNumber) + ++accountCounter;
    return std::to_string(result);
}

Card SetupAccount::createAccount() {
    std::string accNum = generateAccountNumber();
    int pin;
    double balance, dailyLimit;

    std::cout << cSetupPin << accNum << " : ";
    std::cin >> pin;
    std::cout << cEnterInitialBalance;
    std::cin >> balance;
    std::cout << cEnterDailyLimit;
    std::cin >> dailyLimit;

    Account* account = new Account(accNum, pin, balance, dailyLimit);

    std::string cardNumber = generateCardNumber();

    Card card(cardNumber, account);
    cards.push_back(card);

    std::cout << cCardCreated << cardNumber << "\n";
    return card;
}