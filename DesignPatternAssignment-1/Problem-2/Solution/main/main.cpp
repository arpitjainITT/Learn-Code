#include <iostream>
#include "Application.h"
#include "WindowsUIFactory.h"
#include "MacOSUIFactory.h"

int main() {
    std::cout << "Running on Windows:\n";
    Application windowsApp(std::make_unique<WindowsUIFactory>());
    windowsApp.renderUI();

    std::cout << "\nRunning on MacOS:\n";
    Application macApp(std::make_unique<MacOSUIFactory>());
    macApp.renderUI();

    return 0;
}
