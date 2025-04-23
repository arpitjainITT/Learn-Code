#include "IOHandler.h"
#include "BankServer.h"
#include "SetupAccount.h"
#include "ATM.h"
#include <iostream>

void IOHandler::runSession() {
    SetupAccount setup;
    BankServer server;
    ATM atm("ATM-001", maxATMCash, &server);
    Card card("", nullptr);
    bool cardCreated = false;

    int mainChoice;
    do {
        std::cout << cMainMenu;
        std::cin >> mainChoice;

        switch (mainChoice) {
            case 1:
                card = setup.createAccount();
                cardCreated = true;
                break;
            case 2:
                if (!cardCreated) {
                    std::cout << cCreateAccountRequest;
                    break;
                }
                int atmChoice;
                do {
                    std::cout << cATMMenu;
                    std::cin >> atmChoice;

                    switch (atmChoice) {
                        case 1: {
                            int inputPin;
                            double amount;
                            std::cout << cEnterPin;
                            std::cin >> inputPin;
                            std::cout << cEnterWithdrawalAmount;
                            std::cin >> amount;

                            try {
                                atm.withdraw(card, inputPin, amount);
                            } catch (const std::exception& e) {
                                std::cerr << cError << e.what() << std::endl;
                            }
                            break;
                        }
                        case 2:
                            std::cout << cCurrentBalance << card.getAccount()->getBalance() << std::endl;
                            std::cout << cRemainingDailyLimit << card.getAccount()->getRemainingDailyLimit() << std::endl;
                            break;
                        case 3:
                            std::cout << cReturningMainMenu;
                            break;
                        default:
                            std::cout << cInvalidOption;
                    }
                } while (atmChoice != 3);
                break;
            case 3:
                std::cout << cExiting;
                break;
            default:
                std::cout << cInvalidOption;
        }
    } while (mainChoice != 3);
}