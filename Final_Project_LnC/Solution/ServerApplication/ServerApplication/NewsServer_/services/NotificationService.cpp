#include "NotificationService.hpp"
#include "../database/Database.hpp"
#include <iostream>

using json = nlohmann::json;

void NotificationService::setCategoryPreference(int userId, const std::string& category, bool isEnabled) {
    Database::addCategory(category); 
    int categoryId = Database::getCategoryId(category);
    if (categoryId != -1) {
        Database::setCategoryNotificationPreference(userId, categoryId, isEnabled);
    } else {
        std::cerr << "[NotificationService] Failed to resolve category: " << category << "\n";
    }
}

void NotificationService::setKeywordPreference(int userId, const std::string& keyword, bool isEnabled) {
    Database::setKeywordNotificationPreference(userId, keyword, isEnabled);
}

json NotificationService::getUserPreferences(int userId) {
    return Database::getUserNotificationPreferences(userId);
}
