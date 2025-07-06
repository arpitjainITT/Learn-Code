#ifndef NOTIFICATIONSERVICE_H
#define NOTIFICATIONSERVICE_H

#include <string>
#include <unordered_map>
#include <vector>

struct NotificationPreferences {
    std::unordered_map<std::string, bool> categories;
    std::unordered_map<std::string, bool> keywords;
};

struct Notification {
    int id;
    std::string title;
    std::string message;
    std::string category;  
    std::string url;        
    std::string source;    
    bool read;
};

class NotificationService {
public:
    static void setCategoryPreference(int userId, const std::string& category, bool enabled);
    static void setKeywordPreference(int userId, const std::string& keyword, bool enabled);
    static NotificationPreferences getPreferences(int userId);
    static std::vector<Notification> getNotifications(int userId);  
    static void markNotificationAsRead(int notificationId);
};

#endif
