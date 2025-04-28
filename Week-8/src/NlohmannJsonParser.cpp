#include "NlohmannJsonParser.h"
#include <iostream> 
using json = nlohmann::json;

std::pair<std::string, std::string> NlohmannJsonParser::parseCoordinates(const std::string& jsonStr) {
    try {
        auto j = json::parse(jsonStr);

        if (j.is_array() && !j.empty()) {
            const auto& firstResult = j[0];  

            if (firstResult.contains("lat") && firstResult.contains("lon")) {
                std::string lattitude = firstResult["lat"];
                std::string longitude = firstResult["lon"];
                return {lattitude, longitude};
            } else {
                std::cerr << cCoordinatesNotFound;
                return {"", ""};
            }
        } else {
            std::cerr << cUnexpectedJsonFormat;
            return {"", ""};
        }
    } catch (const std::exception& e) {
        std::cerr << cJSONParsingError << e.what() << "\n";
        return {"", ""};
    }
}
