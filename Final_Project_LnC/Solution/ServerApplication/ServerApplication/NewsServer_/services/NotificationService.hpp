#ifndef NOTIFICATION_SERVICE_HPP
#define NOTIFICATION_SERVICE_HPP

#include <string>
#include <pistache/http.h>  

class NotificationService {
public:
    static void configureCategory(int userId, const std::string& category, bool enabled, Pistache::Http::ResponseWriter&& response);
    static void configureKeyword(int userId, const std::string& keyword, Pistache::Http::ResponseWriter&& response);
    static void getUserPreferences(int userId, Pistache::Http::ResponseWriter&& response);
};

#endif 
