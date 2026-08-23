#ifndef EMAIL_SENDER_HPP
#define EMAIL_SENDER_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class EmailSender {
public:
    static bool sendEmail(const std::string& to, const std::string& subject, const std::string& body);
};

#endif 
