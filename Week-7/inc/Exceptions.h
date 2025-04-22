#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include "Constants.h"

class InsufficientFundsException : public std::runtime_error {
public:
    InsufficientFundsException() : std::runtime_error(cInsufficientFunds) {}
};

class ATMCashUnavailableException : public std::runtime_error {
public:
    ATMCashUnavailableException() : std::runtime_error(cATMCashUnavailable) {}
};

class ConnectionFailedException : public std::runtime_error {
public:
    ConnectionFailedException() : std::runtime_error(cConnectionFailed) {}
};

class CardBlockedException : public std::runtime_error {
public:
    CardBlockedException() : std::runtime_error(cCardBlocked) {}
};

class DailyLimitExceededException : public std::runtime_error {
public:
    DailyLimitExceededException() : std::runtime_error(cDailyLimitExceeded) {}
};

#endif 