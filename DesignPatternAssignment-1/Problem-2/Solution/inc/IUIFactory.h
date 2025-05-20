#ifndef IUIFACTORYH
#define IUIFACTORYH

#include <memory>
#include "IButton.h"
#include "ICheckbox.h"
#include "ITextField.h"

class IUIFactory {
public:
    virtual std::unique_ptr<IButton> createButton() const = 0;
    virtual std::unique_ptr<ICheckbox> createCheckbox() const = 0;
    virtual std::unique_ptr<ITextField> createTextField() const = 0;
    virtual ~IUIFactory() = default;
};

#endif 