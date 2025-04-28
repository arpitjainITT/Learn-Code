#ifndef GEOCODEHANDLER_H
#define GEOCODEHANDLER_H

#include "IGeoService.h"

class GeoCodeHandler {
public:
    GeoCodeHandler(IGeoService* geoService);
    void process();
private:
    IGeoService* geoService_;
};

#endif 
