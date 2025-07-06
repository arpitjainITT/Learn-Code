#include "auth_routes.hpp"
#include "../services/UserService.hpp"
#include <pistache/http.h>
#include <nlohmann/json.hpp>
#include "../utils/Strings.hpp"

using namespace Pistache;
using json = nlohmann::json;

// POST /auth/login
Rest::Route::Result loginHandler(const Rest::Request& req, Http::ResponseWriter response) {
    auto body = json::parse(req.body());
    std::string email = body["email"];
    std::string password = body["password"];

    int userId = -1;
    std::string role;
    bool success = UserService::login(email, password, userId, role);

    if (success) {
        json res = {
            {"status", "success"},
            {"user_id", userId},
            {"role", role}
        };
        response.send(Http::Code::Ok, res.dump());
    } else {
        std::cout << Strings::ROUTE_LOGIN_FAILED_LOG << email << ", UserId: " << userId << ", Role: " << role << std::endl;
        response.send(Http::Code::Unauthorized, Strings::ROUTE_LOGIN_INVALID_CREDENTIALS);
    }
    return Rest::Route::Result::Ok;
}

// POST /auth/signup
Rest::Route::Result signupHandler(const Rest::Request& req, Http::ResponseWriter response) {
    auto body = json::parse(req.body());
    std::string username = body["username"];
    std::string email = body["email"];
    std::string password = body["password"];

    bool success = UserService::signup(username, email, password);
    if (success) {
        response.send(Http::Code::Created, Strings::ROUTE_SIGNUP_USER_REGISTERED);
    } else {
        response.send(Http::Code::Bad_Request, Strings::ROUTE_SIGNUP_FAILED);
    }
    return Rest::Route::Result::Ok;
}

void AuthRoutes::setup(Rest::Router& router) {
    using namespace Rest;

    Routes::Post(router, "/auth/login", loginHandler);
    Routes::Post(router, "/auth/signup", signupHandler);
}
