#ifndef APPLICATIONH
#define APPLICATIONH

#include <memory>
#include "IUIFactory.h"

class Application {
public:
    explicit Application(std::unique_ptr<IUIFactory> factory);
    void renderUI() const;

private:
    std::unique_ptr<IUIFactory> factory_;
};

#endif 