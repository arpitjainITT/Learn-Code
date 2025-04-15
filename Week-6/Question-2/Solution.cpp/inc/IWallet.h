#ifndef IWALLET_H
#define IWALLET_H

class IWallet {
public:
    virtual bool withdraw(float amount) = 0;
    virtual void deposit(float amount) = 0;
    virtual float getBalance() const = 0;
    virtual ~IWallet() = default;
};

#endif
