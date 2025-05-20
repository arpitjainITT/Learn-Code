#include "WindowsUIFactory.h"
#include "WindowsComponents.h"

std::unique_ptr<IButton> WindowsUIFactory::createButton() const {
    return std::make_unique<WindowsButton>();
}

std::unique_ptr<ICheckbox> WindowsUIFactory::createCheckbox() const {
    return std::make_unique<WindowsCheckbox>();
}

std::unique_ptr<ITextField> WindowsUIFactory::createTextField() const {
    return std::make_unique<WindowsTextField>();
}
