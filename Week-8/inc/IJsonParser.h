#ifndef IJSONPARSER_H
#define IJSONPARSER_H

#include <string>
#include <utility>

class IJsonParser {
public:
    virtual std::pair<std::string, std::string> parseCoordinates(const std::string& jsonStr) = 0;
    virtual ~IJsonParser() = default;
};


#endif