#ifndef WALLET_H
#define WALLET_H

#include "IWallet.h"

class Wallet : public IWallet {
private:
    float balance;

public:
    Wallet(float initialAmount = 0.0f);
    bool withdraw(float amount) override;
    void deposit(float amount) override;
    float getBalance() const override;
};

#endif
