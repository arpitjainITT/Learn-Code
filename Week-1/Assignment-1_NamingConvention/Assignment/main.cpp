#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp" 

using json = nlohmann::json;

std::string getCountryName(const std::string& countryCode, const json& countryData) {
    if (countryData.contains(countryCode)) {
        const auto& adjacentCountries = countryData[countryCode];
        std::string result;
        for (const auto& country : adjacentCountries) {
            if (!result.empty()) result += ", ";
            result += country.get<std::string>();
        }
        return result;
    }
    return "No data available for this country code.";
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

    std::string adjacentCountriesName = getCountryName(countryCode, countryData);
    std::cout << "Adjacent countries of " << countryCode << " : " << adjacentCountriesName << std::endl;

    return 0;
}
