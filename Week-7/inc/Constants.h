#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const std::string cInsufficientFunds = "Insufficient funds in account.";
const std::string cATMCashUnavailable = "ATM has insufficient cash.";
const std::string cConnectionFailed = "Unable to connect to bank server.";
const std::string cCardBlocked = "Card is blocked after 3 invalid attempts.";
const std::string cDailyLimitExceeded = "Daily withdrawal limit exceeded.";
const std::string cInvalidCard = "Invalid card.";
const std::string cInvalidPin = "Invalid PIN.";

const std::string initialAccountNumber = "6355560000000000"; //Starting point for account numbers
const std::string initialCardNumber = "7384453200000000";    //Starting point for card numbers

const std::string cWithdrawlSuccessful = "Withdrawal successful. Remaining balance: ";

const std::string cMainMenu = "=== MAIN MENU ===\n"
                            "1. Create Account\n"
                            "2. Use ATM\n"
                            "3. Exit\n"
                            "Choose an option: ";

const std::string cATMMenu = "=== ATM MENU ===\n"
                            "1. Withdraw Cash\n"
                            "2. Check Balance\n"
                            "3. Back to Main Menu\n"
                            "Choose an option: ";

const std::string cCreateAccountRequest = "Please create an account first.\n";
const std::string cEnterPin = "Enter PIN: ";
const std::string cEnterWithdrawalAmount = "Enter amount to withdraw: ";
const std::string cError = "Error: ";
const std::string cCurrentBalance = "Current balance: ";
const std::string cRemainingDailyLimit = "Remaining daily limit: ";
const std::string cReturningMainMenu = "Returning to main menu...\n";
const std::string cExiting = "Exiting...\n";
const std::string cInvalidOption = "Invalid option. Please try again !!\n";

const std::string cSetupPin = "SetUp PIN for account Number ";
const std::string cEnterInitialBalance = "Enter initial balance: ";
const std::string cEnterDailyLimit = "Enter daily withdrawal limit: ";
const std::string cCardCreated = "Card Created: ";

const double maxATMCash = 10000.0; // Maximum cash in ATM
const int maxPinAttempts = 3; // Maximum PIN attempts before blocking the card
#endif 