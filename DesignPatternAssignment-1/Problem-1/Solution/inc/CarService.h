#ifndef CARSERVICEH
#define CARSERVICEH

#include "IVehicleService.h"

class CarService : public IVehicleService {
public:
    void performService() const override;
};

#endif 
