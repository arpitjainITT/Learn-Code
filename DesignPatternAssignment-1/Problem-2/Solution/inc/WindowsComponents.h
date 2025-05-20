#ifndef WINDOWSCOMPONENTSH
#define WINDOWSCOMPONENTSH

#include "IButton.h"
#include "ICheckbox.h"
#include "ITextField.h"

class WindowsButton : public IButton {
public:
    void render() const override;
};

class WindowsCheckbox : public ICheckbox {
public:
    void render() const override;
};

class WindowsTextField : public ITextField {
public:
    void render() const override;
};

#endif 