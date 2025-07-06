#include "NotificationService.h"
#include "../utils/HttpClient.h"
#include "../constants/APIEndpoints.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

void NotificationService::setCategoryPreference(int userId, const std::string& category, bool enabled) {
    json body = {
        {"user_id", userId},
        {"category", category},
        {"enabled", enabled}
    };

    HttpClient::post(API::SET_CATEGORY_PREF, body.dump());
}

void NotificationService::setKeywordPreference(int userId, const std::string& keyword, bool enabled) {
    json body = {
        {"user_id", userId},
        {"keyword", keyword},
        {"enabled", enabled}
    };

    HttpClient::post(API::SET_KEYWORD_PREF, body.dump());
}

NotificationPreferences NotificationService::getPreferences(int userId) {
    NotificationPreferences prefs;
    try {
        std::string url = API::GET_PREFERENCES + std::to_string(userId);
        std::string responseStr = HttpClient::get(url);
        auto result = json::parse(responseStr);

        if (result.contains("categories")) {
            for (const auto& catObj : result["categories"]) {
                if (catObj.contains("category") && catObj.contains("enabled")) {
                    prefs.categories[catObj["category"]] = catObj["enabled"];
                }
            }            
        }
        if (result.contains("keywords")) {
            for (const auto& kwObj : result["keywords"]) {
                if (kwObj.contains("keyword") && kwObj.contains("enabled")) {
                    prefs.keywords[kwObj["keyword"]] = kwObj["enabled"];
                }
            }            
        }
    } catch (...) {
        std::cerr << "Failed to fetch notification preferences.\n";
    }

    return prefs;
}

std::vector<Notification> NotificationService::getNotifications(int userId) {
    std::vector<Notification> notifications;

    try {
        std::string url = API::USER_NOTIFICATIONS + std::to_string(userId);
        std::string responseStr = HttpClient::get(url);
        auto result = json::parse(responseStr);

        for (const auto& item : result) {
            Notification n;
            n.id = item["id"];
            n.title = item["title"];
            n.message = item["description"];
            n.category = item.value("category", "General");
            n.url = item["url"];
            n.source = item["source"];
            n.read = item.value("read", false);
            notifications.push_back(n);
        }
    } catch (...) {
        std::cerr << "Failed to fetch notifications.\n";
    }

    return notifications;
}

void NotificationService::markNotificationAsRead(int notificationId) {
    std::string endpoint = API::USER_NOTIFICATIONS + std::to_string(notificationId) + "/read";
    try {
        HttpClient::put(endpoint, ""); 
    } catch (...) {
        std::cerr << "Failed to mark notification as read.\n";
    }
}
