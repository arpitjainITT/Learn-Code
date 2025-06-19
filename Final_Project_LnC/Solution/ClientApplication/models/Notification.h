#pragma once
#include <string>

struct Notification {
    std::string id;
    std::string title;
    std::string message;
    std::string category;
    std::string timestamp;
    bool read;

    Notification(std::string i, std::string t, std::string m, std::string c, std::string ts, bool r)
        : id(i), title(t), message(m), category(c), timestamp(ts), read(r) {}
};
