#include "admin_routes.hpp"
#include "../services/AdminService.hpp"
#include <pistache/http.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <nlohmann/json.hpp>

using namespace Pistache;
using json = nlohmann::json;

// GET /admin/servers
Rest::Route::Result listServersHandler(const Rest::Request& req, Http::ResponseWriter response) {
    json result = AdminService::listExternalServers();
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

// GET /admin/servers/:id
Rest::Route::Result viewServerHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int serverId = req.param(":id").as<int>();
    json result = AdminService::viewExternalServerDetails(serverId);
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

// PUT /admin/servers/:id/key
Rest::Route::Result updateApiKeyHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int serverId = req.param(":id").as<int>();
    auto body = json::parse(req.body());
    std::string newKey = body.value("api_key", "");
    bool ok = AdminService::updateExternalServerApiKey(serverId, newKey);
    response.send(ok ? Http::Code::Ok : Http::Code::Bad_Request);
    return Rest::Route::Result::Ok;
}

// PUT /admin/servers/:id/status
Rest::Route::Result updateStatusHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int serverId = req.param(":id").as<int>();
    auto body = json::parse(req.body());
    std::string status = body.value("status", "");
    bool ok = AdminService::updateExternalServerStatus(serverId, status);
    response.send(ok ? Http::Code::Ok : Http::Code::Bad_Request);
    return Rest::Route::Result::Ok;
}

// POST /admin/category
Rest::Route::Result addCategoryHandler(const Rest::Request& req, Http::ResponseWriter response) {
    auto body = json::parse(req.body());
    std::string category = body.value("category", "");
    bool ok = AdminService::addCategory(category);
    response.send(ok ? Http::Code::Created : Http::Code::Bad_Request);
    return Rest::Route::Result::Ok;
}

void AdminRoutes::setup(Rest::Router& router) {
    using namespace Rest;

    Routes::Get(router, "/admin/servers", listServersHandler);
    Routes::Get(router, "/admin/servers/:id", viewServerHandler);
    Routes::Put(router, "/admin/servers/:id/key", updateApiKeyHandler);
    Routes::Put(router, "/admin/servers/:id/status", updateStatusHandler);
    Routes::Post(router, "/admin/category", addCategoryHandler);
}
