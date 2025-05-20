#ifndef MACOSUIFACTORYH
#define MACOSUIFACTORYH

#include "IUIFactory.h"

class MacOSUIFactory : public IUIFactory {
public:
    std::unique_ptr<IButton> createButton() const override;
    std::unique_ptr<ICheckbox> createCheckbox() const override;
    std::unique_ptr<ITextField> createTextField() const override;
};

#endif 