#include "VehicleServiceFactory.h"
#include "CarService.h"
#include "BikeService.h"
#include "TruckService.h"

#include <algorithm>

std::unique_ptr<IVehicleService> VehicleServiceFactory::createService(const std::string& vehicleType) {
    std::string type = toLower(vehicleType);

    if (type == "car") {
        return std::make_unique<CarService>();
    } else if (type == "bike") {
        return std::make_unique<BikeService>();
    } else if (type == "truck") {
        return std::make_unique<TruckService>();
    }

    return nullptr;
}

std::string VehicleServiceFactory::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
