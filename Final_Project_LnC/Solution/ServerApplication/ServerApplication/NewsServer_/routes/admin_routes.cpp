#include "admin_routes.hpp"
#include "../services/AdminService.hpp"
#include "../utils/JwtUtils.hpp"
#include <pistache/http.h>
#include <pistache/router.h>
#include <nlohmann/json.hpp>

using namespace Pistache;
using json = nlohmann::json;

// All admin handlers require a valid JWT with role == "admin".

// GET /admin/servers
Rest::Route::Result listServersHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    response.send(Http::Code::Ok, AdminService::listExternalServers().dump());
    return Rest::Route::Result::Ok;
}

// GET /admin/servers/:id
Rest::Route::Result viewServerHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    int serverId = req.param(":id").as<int>();
    response.send(Http::Code::Ok, AdminService::viewExternalServerDetails(serverId).dump());
    return Rest::Route::Result::Ok;
}

// PUT /admin/servers/:id/key
Rest::Route::Result updateApiKeyHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    int serverId   = req.param(":id").as<int>();
    auto body      = json::parse(req.body());
    std::string newKey = body.value("api_key", "");
    bool ok = AdminService::updateExternalServerApiKey(serverId, newKey);
    response.send(ok ? Http::Code::Ok : Http::Code::Bad_Request);
    return Rest::Route::Result::Ok;
}

// PUT /admin/servers/:id/status
Rest::Route::Result updateStatusHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    int serverId  = req.param(":id").as<int>();
    auto body     = json::parse(req.body());
    std::string status = body.value("status", "");
    bool ok = AdminService::updateExternalServerStatus(serverId, status);
    response.send(ok ? Http::Code::Ok : Http::Code::Bad_Request);
    return Rest::Route::Result::Ok;
}

// POST /admin/category
Rest::Route::Result addCategoryHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    auto body = json::parse(req.body());
    std::string category = body.value("category", "");
    bool ok = AdminService::addCategory(category);
    response.send(ok ? Http::Code::Created : Http::Code::Bad_Request);
    return Rest::Route::Result::Ok;
}

// POST /admin/categories/hide
Rest::Route::Result hideCategoryHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    auto body = json::parse(req.body());
    AdminService::hideCategory(body.value("category", ""));
    response.send(Http::Code::Ok, R"({"status":"ok","message":"Category hidden."})");
    return Rest::Route::Result::Ok;
}

// POST /admin/categories/unhide
Rest::Route::Result unhideCategoryHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    auto body = json::parse(req.body());
    AdminService::unhideCategory(body.value("category", ""));
    response.send(Http::Code::Ok, R"({"status":"ok","message":"Category unhidden."})");
    return Rest::Route::Result::Ok;
}

// POST /admin/keywords/filter
Rest::Route::Result addFilteredKeywordHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    auto body = json::parse(req.body());
    AdminService::addFilteredKeyword(body.value("keyword", ""));
    response.send(Http::Code::Ok, R"({"status":"ok","message":"Keyword filtered."})");
    return Rest::Route::Result::Ok;
}

// POST /admin/keywords/unfilter
Rest::Route::Result removeFilteredKeywordHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    auto body = json::parse(req.body());
    AdminService::removeFilteredKeyword(body.value("keyword", ""));
    response.send(Http::Code::Ok, R"({"status":"ok","message":"Keyword unfiltered."})");
    return Rest::Route::Result::Ok;
}

// GET /admin/hidden-categories
Rest::Route::Result getHiddenCategoriesHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    response.send(Http::Code::Ok, AdminService::getHiddenCategories().dump());
    return Rest::Route::Result::Ok;
}

// GET /admin/filtered-keywords
Rest::Route::Result getFilteredKeywordsHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int uid; std::string role;
    if (!JwtUtils::requireAdmin(req, response, uid, role)) return Rest::Route::Result::Ok;
    response.send(Http::Code::Ok, AdminService::getFilteredKeywords().dump());
    return Rest::Route::Result::Ok;
}

void AdminRoutes::setup(Rest::Router& router) {
    using namespace Rest;
    Routes::Get(router,  "/admin/servers",              listServersHandler);
    Routes::Get(router,  "/admin/servers/:id",          viewServerHandler);
    Routes::Put(router,  "/admin/servers/:id/key",      updateApiKeyHandler);
    Routes::Put(router,  "/admin/servers/:id/status",   updateStatusHandler);
    Routes::Post(router, "/admin/category",             addCategoryHandler);
    Routes::Post(router, "/admin/categories/hide",      hideCategoryHandler);
    Routes::Post(router, "/admin/categories/unhide",    unhideCategoryHandler);
    Routes::Post(router, "/admin/keywords/filter",      addFilteredKeywordHandler);
    Routes::Post(router, "/admin/keywords/unfilter",    removeFilteredKeywordHandler);
    Routes::Get(router,  "/admin/hidden-categories",    getHiddenCategoriesHandler);
    Routes::Get(router,  "/admin/filtered-keywords",    getFilteredKeywordsHandler);
}
