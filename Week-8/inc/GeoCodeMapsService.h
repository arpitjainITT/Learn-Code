#ifndef GeoCodeMapsService_H
#define GeoCodeMapsService_H

#include "IGeoService.h"
#include "IHttpClient.h"
#include "IJsonParser.h"

class GeoCodeMapsService : public IGeoService {
public:
    GeoCodeMapsService(IHttpClient* httpClient, IJsonParser* jsonParser, const std::string& apiKey);
    std::pair<std::string, std::string> getCoordinates(const std::string& place) override;
    std::string encodePlaceName(const std::string& place) override;

private:
    IHttpClient* httpClient_;
    IJsonParser* jsonParser_;
    std::string apiKey_;
};

#endif
