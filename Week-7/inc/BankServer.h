#ifndef BANKSERVER_H
#define BANKSERVER_H

#include "IBankServer.h"

class BankServer : public IBankServer {
public:
    bool isConnected() const override {
        return true; //During testing this can be set to false to simulate a connection failure
    }
};

#endif 