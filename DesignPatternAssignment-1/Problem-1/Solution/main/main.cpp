#include <iostream>
#include <string>
#include "VehicleServiceFactory.h"

int main() {
    std::string input;
    std::cout << "Enter vehicle type (car, bike, truck): ";
    std::getline(std::cin, input);

    auto service = VehicleServiceFactory::createService(input);

    if (service) {
        service->performService();
    } else {
        std::cout << "Invalid vehicle type. Please enter 'car', 'bike', or 'truck'.\n";
    }

    return 0;
}
