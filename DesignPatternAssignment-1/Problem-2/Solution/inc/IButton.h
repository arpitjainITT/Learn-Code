#ifndef BUTTONH
#define BUTTONH

class IButton {
public:
    virtual void render() const = 0;
    virtual ~IButton() = default;
};

#endif 