#ifndef TRUCKSERVICEH
#define TRUCKSERVICEH

#include "IVehicleService.h"

class TruckService : public IVehicleService {
public:
    void performService() const override;
};

#endif 
