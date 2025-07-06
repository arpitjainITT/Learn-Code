#include "article_routes.hpp"
#include "../services/ArticleService.hpp"
#include <pistache/http.h>
#include <nlohmann/json.hpp>
#include "../utils/Strings.hpp"

using namespace Pistache;
using json = nlohmann::json;

// GET /articles
Rest::Route::Result getAllArticlesHandler(const Rest::Request& req, Http::ResponseWriter response) {
    json result = ArticleService::getAllArticles();
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

// GET /articles/:id
Rest::Route::Result getArticleByIdHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int articleId = req.param(":id").as<int>();
    json result = ArticleService::getArticleById(articleId);
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

// GET /articles/category/:category
Rest::Route::Result getArticlesByCategoryHandler(const Rest::Request& req, Http::ResponseWriter response) {
    std::string category = req.param(":category").as<std::string>();
    json result = ArticleService::getArticlesByCategory(category);
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

// POST /articles/save
Rest::Route::Result saveArticleHandler(const Rest::Request& req, Http::ResponseWriter response) {
    auto body = json::parse(req.body());
    int userId = body["user_id"];
    int articleId = body["article_id"];

    ArticleService::saveArticle(userId, articleId);
    response.send(Http::Code::Ok, Strings::ROUTE_ARTICLE_SAVED);
    return Rest::Route::Result::Ok;
}

// DELETE /articles/save
Rest::Route::Result deleteSavedArticleHandler(const Rest::Request& req, Http::ResponseWriter response) {
    auto body = json::parse(req.body());
    int userId = body.value("userId", -1);
    int articleId = body.value("articleId", -1);
    ArticleService::deleteSavedArticle(userId, articleId);
    response.send(Http::Code::Ok, Strings::ROUTE_ARTICLE_UNSAVED);
    return Rest::Route::Result::Ok;
}

// GET /articles/saved/:userId
Rest::Route::Result getSavedArticlesHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int userId = req.param(":userId").as<int>();
    json result = ArticleService::getSavedArticlesForUser(userId);
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

// POST /articles/:id/like
Rest::Route::Result likeArticleHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int articleId = req.param(":id").as<int>();
    auto body = json::parse(req.body());
    int userId = body["user_id"];
    ArticleService::likeArticle(userId, articleId);
    response.send(Http::Code::Ok, Strings::ROUTE_ARTICLE_LIKED);
    return Rest::Route::Result::Ok;
}

// POST /articles/:id/dislike
Rest::Route::Result dislikeArticleHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int articleId = req.param(":id").as<int>();
    auto body = json::parse(req.body());
    int userId = body["user_id"];
    ArticleService::dislikeArticle(userId, articleId);
    response.send(Http::Code::Ok, Strings::ROUTE_ARTICLE_DISLIKED);
    return Rest::Route::Result::Ok;
}

// GET /articles/search?keyword=&startDate=&endDate=&sort=
Rest::Route::Result searchArticlesHandler(const Rest::Request& req, Http::ResponseWriter response) {
    auto query = req.query();
    std::string keyword   = query.get("keyword").value_or("");
    std::string startDate = query.get("startDate").value_or("");
    std::string endDate   = query.get("endDate").value_or("");
    std::string sortBy    = query.get("sort").value_or("");  // "likes", "dislikes", or ""


    json result = ArticleService::searchArticles(keyword, startDate, endDate, sortBy);
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

// GET /articles/:id/reactions
Rest::Route::Result getReactionStatsHandler(const Rest::Request& req, Http::ResponseWriter response) {
    int articleId = req.param(":id").as<int>();
    json result = ArticleService::getReactionStats(articleId);
    response.send(Http::Code::Ok, result.dump());
    return Rest::Route::Result::Ok;
}

void ArticleRoutes::setup(Rest::Router& router) {
    using namespace Rest;

    Routes::Get(router, "/articles", getAllArticlesHandler);
    Routes::Get(router, "/articles/:id", getArticleByIdHandler);
    Routes::Get(router, "/articles/category/:category", getArticlesByCategoryHandler);
    Routes::Post(router, "/articles/save", saveArticleHandler);
    Routes::Delete(router, "/articles/save", deleteSavedArticleHandler);
    Routes::Get(router, "/articles/saved/:userId", getSavedArticlesHandler);
    Routes::Post(router, "/articles/:id/like", likeArticleHandler);
    Routes::Post(router, "/articles/:id/dislike", dislikeArticleHandler);
    Routes::Get(router, "/articles/search", searchArticlesHandler);
    Routes::Get(router, "/articles/:id/reactions", getReactionStatsHandler);
}
