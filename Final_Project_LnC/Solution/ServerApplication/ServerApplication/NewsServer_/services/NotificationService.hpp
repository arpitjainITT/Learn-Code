#ifndef NOTIFICATIONSERVICE_HPP
#define NOTIFICATIONSERVICE_HPP

#include <nlohmann/json.hpp>
#include <string>

class NotificationService {
public:
    static void setCategoryPreference(int userId, const std::string& category, bool isEnabled);
    static void setKeywordPreference(int userId, const std::string& keyword, bool isEnabled);
    static nlohmann::json getUserPreferences(int userId);
};

#endif 
