#include "notification_routes.hpp"
#include "../services/NotificationService.hpp"
#include <pistache/http.h>
#include <nlohmann/json.hpp>

using namespace Pistache;
using json = nlohmann::json;

// POST /notifications/category
Rest::Route::Result setCategoryPrefHandler(const Rest::Request& req, Http::ResponseWriter response) {
    auto body = json::parse(req.body());
    int userId = body["user_id"];
    std::string category = body["category"];
    bool enabled = body["enabled"];
    
    NotificationService::setCategoryPreference(userId, category, enabled);
    response.send(Http::Code::Ok, "Category preference updated.");
    return Rest::Route::Result::Ok;
}

// POST /notifications/keyword
Rest::Route::Result setKeywordPrefHandler(const Rest::Request& req, Http::ResponseWriter response) {
    auto body = json::parse(req.body());
    int userId = body["user_id"];
    std::string keyword = body["keyword"];
    bool enabled = body["enabled"];
    
    NotificationService::setKeywordPreference(userId, keyword, enabled);
    response.send(Http::Code::Ok, "Keyword preference updated.");
    return Rest::Route::Result::Ok;
}

// GET /notifications/preferences/:userId
Rest::Route::Result getPreferencesHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int userId = req.param(":userId").as<int>();
    json result = NotificationService::getUserPreferences(userId);
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

void NotificationRoutes::setup(Rest::Router& router) {
    using namespace Rest;

    Routes::Post(router, "/notifications/category", setCategoryPrefHandler);
    Routes::Post(router, "/notifications/keyword", setKeywordPrefHandler);
    Routes::Get(router, "/notifications/preferences/:userId", getPreferencesHandler);
}
