#include "Wallet.h"

Wallet::Wallet(float initialAmount) : balance(initialAmount) {}

bool Wallet::withdraw(float amount) {
    if (balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

void Wallet::deposit(float amount) {
    balance += amount;
}

float Wallet::getBalance() const {
    return balance;
}
