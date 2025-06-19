#ifndef EMAILSENDER_HPP
#define EMAILSENDER_HPP

#include <string>

class EmailSender {
public:
    static bool send(const std::string& to, const std::string& subject, const std::string& body);
};

#endif
