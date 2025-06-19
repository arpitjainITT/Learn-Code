#include "auth_routes.hpp"
#include "../services/UserService.hpp"
#include "../services/ArticleService.hpp"
#include <nlohmann/json.hpp>

using namespace Pistache;
using json = nlohmann::json;

void register_auth_routes(Rest::Router& router) {
    
    Rest::Routes::Post(router, "/auth/signup",
        [](const Rest::Request& req, Http::ResponseWriter response) -> Rest::Route::Result {
            try {
                auto body = json::parse(req.body(), nullptr, false);
                if (body.is_discarded()) {
                    response.send(Http::Code::Bad_Request, "Invalid JSON");
                    return Rest::Route::Result::Ok;
                }
                UserService::signup(req, std::move(response));
            } catch (const std::exception& e) {
                response.send(Http::Code::Bad_Request, std::string("Error parsing request: ") + e.what());
            }
            return Rest::Route::Result::Ok;
        }
    );

   
    Rest::Routes::Post(router, "/auth/login",
        [](const Rest::Request& req, Http::ResponseWriter response) -> Rest::Route::Result {
            try {
                auto body = json::parse(req.body(), nullptr, false);
                if (body.is_discarded()) {
                    response.send(Http::Code::Bad_Request, "Invalid JSON");
                    return Rest::Route::Result::Ok;
                }
                UserService::login(req, std::move(response));
            } catch (const std::exception& e) {
                response.send(Http::Code::Bad_Request, std::string("Error parsing request: ") + e.what());
            }
            return Rest::Route::Result::Ok;
        }
    );

    Rest::Routes::Post(router, "/articles/save",
        [](const Rest::Request& req, Http::ResponseWriter response) -> Rest::Route::Result {
            try {
                ArticleService::saveArticle(req, std::move(response));
            } catch (const std::exception& e) {
                response.send(Http::Code::Bad_Request, std::string("Error parsing request: ") + e.what());
            }
            return Rest::Route::Result::Ok;
        }
    );

    Rest::Routes::Get(router, "/articles/saved",
        [](const Rest::Request& req, Http::ResponseWriter response) -> Rest::Route::Result {
            try {
                ArticleService::getSavedArticles(req, std::move(response));
            } catch (const std::exception& e) {
                response.send(Http::Code::Bad_Request, std::string("Invalid query: ") + e.what());
            }
            return Rest::Route::Result::Ok;
        }
    );

    Rest::Routes::Delete(router, "/articles/saved/:id",
        [](const Rest::Request& req, Http::ResponseWriter response) -> Rest::Route::Result {
            try {
                ArticleService::deleteSavedArticle(req, std::move(response));
            } catch (const std::exception& e) {
                response.send(Http::Code::Bad_Request, std::string("Invalid saved article ID: ") + e.what());
            }
            return Rest::Route::Result::Ok;
        }
    );
}
