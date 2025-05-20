#include "WindowsComponents.h"
#include <iostream>

void WindowsButton::render() const {
    std::cout << "Rendering Windows Button\n";
}

void WindowsCheckbox::render() const {
    std::cout << "Rendering Windows Checkbox\n";
}

void WindowsTextField::render() const {
    std::cout << "Rendering Windows TextField\n";
}
