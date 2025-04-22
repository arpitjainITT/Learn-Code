#include "Account.h"
#include "Exceptions.h"

Account::Account(std::string accountNo, int pinNo, double totalBalance, double limit)
    : accountNumber(accountNo), pin(pinNo), balance(totalBalance), dailyLimit(limit), withdrawnToday(0), pinAttempts(0) {}

bool Account::verifyPIN(int inputPIN) {
    if (pin == inputPIN) {
        pinAttempts = 0;
        return true;
    } else {
        pinAttempts++;
        return false;
    }
}

bool Account::isBlocked() const { 
    return pinAttempts >= maxPinAttempts; 
}

double Account::getBalance() const { 
    return balance; 
}

double Account::getRemainingDailyLimit() const { 
    return dailyLimit - withdrawnToday; 
}

void Account::withdraw(double amount) {
    if (withdrawnToday + amount > dailyLimit) {
        throw DailyLimitExceededException();
    }

    if (amount > balance) {
        throw InsufficientFundsException();
    }

    balance -= amount;
    withdrawnToday += amount;
}