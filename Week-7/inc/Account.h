#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include "Constants.h"

class Account {
private:
    std::string accountNumber;
    int pin;
    double balance;
    double dailyLimit;
    double withdrawnToday;
    int pinAttempts;
public:
    Account(std::string accountNo, int pinNo, double totalBalance, double limit);

    bool verifyPIN(int inputPIN);
    bool isBlocked() const;
    double getBalance() const;
    double getRemainingDailyLimit() const;
    void withdraw(double amount);
};

#endif