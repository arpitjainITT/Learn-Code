#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp" 

using json = nlohmann::json;

std::string getCountryName(const std::string& countryCode, const json& countryData) {
    return countryData.value(countryCode, "Invalid country code.");
}

int main() {

    std::ifstream file("./countries.json");
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the JSON file." << std::endl;
        return 1;
    }

    json countryData;
    file >> countryData;  
    file.close();

    std::string countryCode;
    std::cout << "Enter country code (e.g., IN, US, NZ): ";
    std::cin >> countryCode;

    for (char& c : countryCode) {
        c = toupper(c);
    }

    std::string countryName = getCountryName(countryCode, countryData);
    std::cout << "Country: " << countryName << std::endl;

    return 0;
}
