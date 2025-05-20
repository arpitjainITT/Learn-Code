#include "MacOSUIFactory.h"
#include "MacOSComponents.h"

std::unique_ptr<IButton> MacOSUIFactory::createButton() const {
    return std::make_unique<MacOSButton>();
}

std::unique_ptr<ICheckbox> MacOSUIFactory::createCheckbox() const {
    return std::make_unique<MacOSCheckbox>();
}

std::unique_ptr<ITextField> MacOSUIFactory::createTextField() const {
    return std::make_unique<MacOSTextField>();
}
