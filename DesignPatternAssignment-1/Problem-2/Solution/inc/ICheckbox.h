#ifndef ICHECKBOXH
#define ICHECKBOXH

class ICheckbox {
public:
    virtual void render() const = 0;
    virtual ~ICheckbox() = default;
};

#endif 