#pragma once

class BaseMenu {
public:
    virtual void show() = 0;
    virtual ~BaseMenu() = default;
};
