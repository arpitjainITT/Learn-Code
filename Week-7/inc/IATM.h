#ifndef IATM_H
#define IATM_H

#include "Card.h"

class IATM {
public:
    virtual void withdraw(Card& card, int pin, double amount) = 0;
    virtual ~IATM() {}
};

#endif 