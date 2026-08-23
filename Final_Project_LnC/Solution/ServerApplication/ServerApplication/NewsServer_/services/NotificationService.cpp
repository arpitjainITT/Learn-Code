#include "NotificationService.hpp"
#include "../database/DataBase.hpp"
#include <iostream>
#include "../utils/Strings.hpp"

using json = nlohmann::json;

void NotificationService::setCategoryPreference(int userId, const std::string& category, bool isEnabled) {
    Database::addCategory(category); 
    int categoryId = Database::getCategoryId(category);
    if (categoryId != -1) {
        Database::setCategoryNotificationPreference(userId, categoryId, isEnabled);
        if (isEnabled) {
            Database::addNotificationsForCategory(userId, category);
        }
    } else {
        std::cerr << Strings::NOTIF_SERVICE_FAILED_RESOLVE_CATEGORY << category << "\n";
    }
}

void NotificationService::setKeywordPreference(int userId, const std::string& keyword, bool isEnabled) {
    Database::setKeywordNotificationPreference(userId, keyword, isEnabled);
    if (isEnabled) {
        Database::filterNotificationsByKeyword(userId, keyword);
    }
}

json NotificationService::getUserPreferences(int userId) {
    return Database::getUserNotificationPreferences(userId);
}

json NotificationService::getDeliveredNotifications(int userId) {
    return Database::getDeliveredNotifications(userId);
}
