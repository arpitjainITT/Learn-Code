#include "notification_routes.hpp"
#include "../services/NotificationService.hpp"
#include <nlohmann/json.hpp>

using namespace Pistache;
using json = nlohmann::json;

void register_notification_routes(Rest::Router& router) {
    using namespace Rest;

    Routes::Post(router, "/notifications/category", [](const Request& req, Http::ResponseWriter response) {
        try {
            auto jsonBody = json::parse(req.body(), nullptr, false);
            if (jsonBody.is_discarded() ||
                !jsonBody.contains("user_id") ||
                !jsonBody.contains("category") ||
                !jsonBody.contains("enabled")) {
                response.send(Http::Code::Bad_Request, "Missing required fields: user_id, category, enabled");
                return Route::Result::Ok;
            }

            int userId = jsonBody["user_id"];
            std::string category = jsonBody["category"];
            bool enabled = jsonBody["enabled"];

            NotificationService::configureCategory(userId, category, enabled, std::move(response));
        } catch (const std::exception& e) {
            response.send(Http::Code::Bad_Request, std::string("Invalid request: ") + e.what());
        }
        return Route::Result::Ok;
    });

    Routes::Post(router, "/notifications/keyword", [](const Request& req, Http::ResponseWriter response) {
        try {
            auto jsonBody = json::parse(req.body(), nullptr, false);
            if (jsonBody.is_discarded() ||
                !jsonBody.contains("user_id") ||
                !jsonBody.contains("keyword")) {
                response.send(Http::Code::Bad_Request, "Missing required fields: user_id, keyword");
                return Route::Result::Ok;
            }

            int userId = jsonBody["user_id"];
            std::string keyword = jsonBody["keyword"];

            NotificationService::configureKeyword(userId, keyword, std::move(response));
        } catch (const std::exception& e) {
            response.send(Http::Code::Bad_Request, std::string("Invalid request: ") + e.what());
        }
        return Route::Result::Ok;
    });

    Routes::Get(router, "/notifications/prefs", [](const Request& req, Http::ResponseWriter response) {
        try {
            int userId = std::stoi(req.query().get("user_id").value_or("0"));
            NotificationService::getUserPreferences(userId, std::move(response));
        } catch (const std::exception& e) {
            response.send(Http::Code::Bad_Request, std::string("Invalid user_id: ") + e.what());
        }
        return Route::Result::Ok;
    });
}
