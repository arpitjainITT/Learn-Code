#ifndef IVEHICLESERVICEH
#define IVEHICLESERVICEH

class IVehicleService {
public:
    virtual void performService() const = 0;
    virtual ~IVehicleService() = default;
};

#endif 
