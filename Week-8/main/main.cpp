#include "CurlHttpClient.h"
#include "Constants.h"
#include "NlohmannJsonParser.h"
#include "GeoCodeMapsService.h"
#include "GeoCodeHandler.h"

int main() {
    CurlHttpClient httpClient;
    NlohmannJsonParser jsonParser;
    GeoCodeMapsService geoService(&httpClient, &jsonParser, API_KEY);
    GeoCodeHandler handler(&geoService);

    handler.process();

    return 0;
}
