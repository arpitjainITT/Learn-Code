#ifndef BIKESERVICEH
#define BIKESERVICEH

#include "IVehicleService.h"

class BikeService : public IVehicleService {
public:
    void performService() const override;
};

#endif 
