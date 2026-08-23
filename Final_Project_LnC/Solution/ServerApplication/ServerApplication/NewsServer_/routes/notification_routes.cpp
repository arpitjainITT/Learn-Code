#include "notification_routes.hpp"
#include "../services/NotificationService.hpp"
#include "../database/DataBase.hpp"
#include "../utils/JwtUtils.hpp"
#include <pistache/http.h>
#include <nlohmann/json.hpp>
#include "../utils/Strings.hpp"

using namespace Pistache;
using json = nlohmann::json;

// All notification endpoints require a valid authenticated user.

// POST /notifications/category
Rest::Route::Result setCategoryPrefHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int userId; std::string role;
    if (!JwtUtils::requireAuth(req, response, userId, role)) return Rest::Route::Result::Ok;

    auto body = json::parse(req.body());
    std::string category = body["category"];
    bool enabled         = body["enabled"];

    NotificationService::setCategoryPreference(userId, category, enabled);
    response.send(Http::Code::Ok, Strings::ROUTE_CATEGORY_PREF_UPDATED);
    return Rest::Route::Result::Ok;
}

// POST /notifications/keyword
Rest::Route::Result setKeywordPrefHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int userId; std::string role;
    if (!JwtUtils::requireAuth(req, response, userId, role)) return Rest::Route::Result::Ok;

    auto body = json::parse(req.body());
    std::string keyword = body["keyword"];
    bool enabled        = body["enabled"];

    NotificationService::setKeywordPreference(userId, keyword, enabled);
    response.send(Http::Code::Ok, Strings::ROUTE_KEYWORD_PREF_UPDATED);
    return Rest::Route::Result::Ok;
}

// GET /notifications/preferences/:userId
Rest::Route::Result getPreferencesHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int tokenUserId; std::string role;
    if (!JwtUtils::requireAuth(req, response, tokenUserId, role)) return Rest::Route::Result::Ok;

    int paramUserId = req.param(":userId").as<int>();
    if (role != "admin" && tokenUserId != paramUserId) {
        response.send(Http::Code::Forbidden,
                      R"({"status":"forbidden","message":"Cannot access another user's preferences"})");
        return Rest::Route::Result::Ok;
    }

    json result = NotificationService::getUserPreferences(paramUserId);
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

// GET /notifications/:userId
Rest::Route::Result getDeliveredNotificationsHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int tokenUserId; std::string role;
    if (!JwtUtils::requireAuth(req, response, tokenUserId, role)) return Rest::Route::Result::Ok;

    int paramUserId = req.param(":userId").as<int>();
    if (role != "admin" && tokenUserId != paramUserId) {
        response.send(Http::Code::Forbidden,
                      R"({"status":"forbidden","message":"Cannot access another user's notifications"})");
        return Rest::Route::Result::Ok;
    }

    json result = NotificationService::getDeliveredNotifications(paramUserId);
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

// POST /notifications/markread/:userId
Rest::Route::Result markNotificationsAsReadHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int tokenUserId; std::string role;
    if (!JwtUtils::requireAuth(req, response, tokenUserId, role)) return Rest::Route::Result::Ok;

    int paramUserId = req.param(":userId").as<int>();
    if (role != "admin" && tokenUserId != paramUserId) {
        response.send(Http::Code::Forbidden,
                      R"({"status":"forbidden","message":"Cannot mark another user's notifications"})");
        return Rest::Route::Result::Ok;
    }

    Database::markNotificationsAsRead(paramUserId);
    response.send(Http::Code::Ok, Strings::ROUTE_NOTIFICATIONS_MARKED_READ);
    return Rest::Route::Result::Ok;
}

void NotificationRoutes::setup(Rest::Router& router) {
    using namespace Rest;
    Routes::Post(router, "/notifications/category",            setCategoryPrefHandler);
    Routes::Post(router, "/notifications/keyword",             setKeywordPrefHandler);
    Routes::Get(router,  "/notifications/preferences/:userId", getPreferencesHandler);
    Routes::Get(router,  "/notifications/:userId",             getDeliveredNotificationsHandler);
    Routes::Post(router, "/notifications/markread/:userId",    markNotificationsAsReadHandler);
}
