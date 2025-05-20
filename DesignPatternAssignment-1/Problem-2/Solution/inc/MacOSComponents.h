#ifndef MACOSCOMPONENTSH
#define MACOSCOMPONENTSH

#include "IButton.h"
#include "ICheckbox.h"
#include "ITextField.h"

class MacOSButton : public IButton {
public:
    void render() const override;
};

class MacOSCheckbox : public ICheckbox {
public:
    void render() const override;
};

class MacOSTextField : public ITextField {
public:
    void render() const override;
};

#endif 