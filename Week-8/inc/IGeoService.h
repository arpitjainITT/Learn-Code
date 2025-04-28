#ifndef IGEOSERVICE_H
#define IGEOSERVICE_H

#include <string>
#include <utility>

class IGeoService {
public:
    virtual std::pair<std::string, std::string> getCoordinates(const std::string& place) = 0;
    virtual std::string encodePlaceName(const std::string& place) = 0;
    virtual ~IGeoService() = default;
};

#endif
