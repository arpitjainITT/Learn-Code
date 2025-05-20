#ifndef VEHICLESERVICEFACTORYH
#define VEHICLESERVICEFACTORYH

#include <memory>
#include <string>
#include "IVehicleService.h"

class VehicleServiceFactory {
public:
    static std::unique_ptr<IVehicleService> createService(const std::string& vehicleType);

private:
    static std::string toLower(const std::string& str);
};

#endif
