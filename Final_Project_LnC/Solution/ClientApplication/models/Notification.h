#pragma once
#include <string>

struct Notification {
    int id;
    std::string title;
    std::string message;    // Corresponds to `description` from server
    std::string category;
    std::string url;
    std::string source;
    bool read;

    Notification(int i, const std::string& t, const std::string& m,
                 const std::string& c, const std::string& u,
                 const std::string& s, bool r)
        : id(i), title(t), message(m), category(c), url(u), source(s), read(r) {}
};
