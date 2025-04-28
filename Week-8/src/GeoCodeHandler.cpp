#include "GeoCodeHandler.h"
#include "Constants.h"
#include <iostream>

GeoCodeHandler::GeoCodeHandler(IGeoService* geoService)
    : geoService_(geoService) {}

void GeoCodeHandler::process() {
    std::string place;
    std::cout << cEnterPlace;
    std::getline(std::cin, place);

    auto [latitude, longitude] = geoService_->getCoordinates(place);
    if (latitude.empty() || longitude.empty()) {
        std::cout << cInvalidCoordinates;
    } else {
        std::cout << cLatitude << latitude << "\n";
        std::cout << cLongitude << longitude << "\n";
    }
}
