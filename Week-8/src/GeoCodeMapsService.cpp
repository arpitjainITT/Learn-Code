#include "GeoCodeMapsService.h"
#include <sstream>
#include <iomanip>
#include <iostream>

GeoCodeMapsService::GeoCodeMapsService(IHttpClient* httpClient, IJsonParser* jsonParser, const std::string& apiKey)
    : httpClient_(httpClient), jsonParser_(jsonParser), apiKey_(apiKey) {}

std::string GeoCodeMapsService::encodePlaceName(const std::string& place) {
    std::ostringstream placeStringStream;
    for (char c : place) {
        if (c == ' ') {
            placeStringStream << "%20";
        } else {
            placeStringStream << c;
        }
    }
    return placeStringStream.str();
}

std::pair<std::string, std::string> GeoCodeMapsService::getCoordinates(const std::string& place) {
    std::string url = "https://geocode.maps.co/search?q=" + encodePlaceName(place) + "&api_key=" + apiKey_;

    std::string response = httpClient_->get(url);
    return jsonParser_->parseCoordinates(response);
}
