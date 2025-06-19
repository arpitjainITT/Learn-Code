#include "article_routes.hpp"
#include "../services/ArticleService.hpp"

using namespace Pistache;

void register_article_routes(Rest::Router& router) {
    using namespace Rest;

    Routes::Get(router, "/articles", Routes::bind(&ArticleService::getAllArticles));

    Routes::Get(router, "/articles/byCategory/:category", Routes::bind(&ArticleService::getArticlesByCategory));

    Routes::Get(router, "/articles/:id", Routes::bind(&ArticleService::getArticleById));

    Routes::Post(router, "/articles/save", Routes::bind(&ArticleService::saveArticle));
}
