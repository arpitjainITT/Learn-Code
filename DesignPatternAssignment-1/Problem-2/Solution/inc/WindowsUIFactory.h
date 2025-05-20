#ifndef WINDOWSUIFACTORYH
#define WINDOWSUIFACTORYH

#include "IUIFactory.h"

class WindowsUIFactory : public IUIFactory {
public:
    std::unique_ptr<IButton> createButton() const override;
    std::unique_ptr<ICheckbox> createCheckbox() const override;
    std::unique_ptr<ITextField> createTextField() const override;
};

#endif 