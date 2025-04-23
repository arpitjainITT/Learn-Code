#ifndef ATM_H
#define ATM_H

#include "IATM.h"
#include "IBankServer.h"

class ATM : public IATM {
private:
    std::string atmId;
    double availableCash;
    IBankServer* server;
public:
    ATM(std::string id, double cash, IBankServer* bankServer);
    void withdraw(Card& card, int pin, double amount) override;
};

#endif