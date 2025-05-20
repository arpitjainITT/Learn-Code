#ifndef ITextFieldH
#define ITextFieldH

class ITextField {
public:
    virtual void render() const = 0;
    virtual ~ITextField() = default;
};

#endif 