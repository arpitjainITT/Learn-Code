#pragma once
#include "IJsonParser.h"
#include "Constants.h"
#include <json.hpp>

class NlohmannJsonParser : public IJsonParser {
public:
    std::pair<std::string, std::string> parseCoordinates(const std::string& jsonStr) override;
};
